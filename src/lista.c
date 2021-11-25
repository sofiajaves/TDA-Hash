#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define EXITO 0
#define ERROR -1

/*
 * Struct creado para guardar los elementos del arbol durante los recorridos
 */

typedef struct conjunto{
  void** array;
  char** claves;
  size_t tamanio;
  size_t posicion;
}conjunto_t;



lista_t* lista_crear(){
    return calloc(1,sizeof(lista_t));
}


int posicion_por_clave(lista_t* lista, char* clave){

    lista_iterador_t* iterador = lista_iterador_crear(lista);
    if(!iterador)
        return ERROR;

    bool se_encontro = false;
    int posicion = 0;

    for(;lista_iterador_tiene_siguiente(iterador) && !se_encontro; lista_iterador_avanzar(iterador)){
        char* clave_actual = lista_iterador_clave_actual(iterador);

        if(strcmp(clave_actual,clave) == 0)
            se_encontro = true;
        else
            posicion++;
    }

    lista_iterador_destruir(iterador);
    return (se_encontro ? posicion : ERROR);
}

/*
 * Pre: Recibe una posicion y un nodo de inicio
 * Post: Recorrera la lista hasta encontrar el nodo en la posicion deseada, devolvera un puntero a ese nodo
 */
nodo_t* recorrer_lista(nodo_t* nodo, int posicion){

    return (posicion == 0 ? nodo : recorrer_lista(nodo->siguiente,posicion-1));
}

int actualizar_elemento(lista_t* lista, void* elemento, char* clave){

    int posicion = posicion_por_clave(lista, clave);

    nodo_t* nodo_a_actualizar = recorrer_lista(lista->nodo_inicio, posicion);
    if(!nodo_a_actualizar)
        return ERROR;

    nodo_a_actualizar->elemento = elemento;
    return EXITO;
}

lista_t* lista_insertar(lista_t* lista, void* elemento, char* clave){
    if(!lista)
        return NULL;

    if(lista_contiene_clave(lista,clave)){
        int resultado = actualizar_elemento(lista, elemento, clave);
        return (resultado == EXITO ? lista : NULL);
    }

    nodo_t* nodo = calloc(1,sizeof(nodo_t));
    if(!nodo)
        return NULL;
    nodo->elemento = elemento;
    nodo->clave = clave;

    if(lista->nodo_fin)
        lista->nodo_fin->siguiente = nodo;
    
    lista->nodo_fin = nodo;

    if(lista_vacia(lista))
        lista->nodo_inicio = nodo;

    (lista->cantidad)++;
    return lista;
}

/*
 * Pre: Recibira el elemento para insertar, la posicion y una referencia al nodo para insertar
 * Post: Insertara el elemento en la lista con una posicion dada
 */
int nodo_insertar_en_posicion(nodo_t* nodo, void* elemento, int posicion){
    if(posicion-1 > 0)
        return nodo_insertar_en_posicion(nodo->siguiente, elemento, posicion-1);
    
    nodo_t* nodo_auxiliar = calloc(1,sizeof(nodo_t));
    if(!nodo_auxiliar)
        return ERROR;

    nodo_auxiliar->siguiente = nodo->siguiente;
    nodo->siguiente = nodo_auxiliar;
    nodo_auxiliar->elemento = elemento;
    return EXITO;
}

int lista_quitar(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return ERROR;

    nodo_t* nodo_fin_nuevo = NULL;

    if(lista_tamanio(lista) > 1){
        nodo_fin_nuevo = recorrer_lista(lista->nodo_inicio, (int)(lista->cantidad-2));
        nodo_fin_nuevo->siguiente = NULL;

    } else
        lista->nodo_inicio = NULL;

    free(lista->nodo_fin);
    lista->nodo_fin = nodo_fin_nuevo;
    lista->cantidad--;

    return EXITO;
}

int lista_quitar_por_clave(lista_t* lista, char* clave){
    if(!lista || lista_vacia(lista))
        return ERROR;

    int posicion = posicion_por_clave(lista, clave);
    if(posicion == ERROR)
        return ERROR;

    if(posicion >= lista->cantidad-1)
        return lista_quitar(lista);

    //void* elemento = NULL;
    nodo_t* nodo_a_borrar = NULL;

    if(posicion == 0){
        nodo_a_borrar = lista->nodo_inicio;
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
    
    } else {

        nodo_t* nodo_anterior = recorrer_lista(lista->nodo_inicio, (int)posicion-1);
        nodo_a_borrar = nodo_anterior->siguiente;
        nodo_anterior->siguiente = nodo_a_borrar->siguiente;

    }

    //elemento = nodo_a_borrar->elemento;
    free(nodo_a_borrar);
    lista->cantidad--; 
    return EXITO;
}

/*
 * Pre: Recibira un nodo y una posicion
 * Post: Devolvera el elemento que se encuentra en la posicion deseada de la lista
 */
void* elemento_en_posicion(nodo_t* nodo, int posicion){
    if(posicion == 0)
        return nodo->elemento;
    return elemento_en_posicion(nodo->siguiente, posicion-1);
}

void* lista_elemento_por_clave(lista_t* lista, char* clave){
    if(!lista)
        return NULL;

    int posicion = posicion_por_clave(lista, clave);
    if(posicion == ERROR)
        return NULL;

    return elemento_en_posicion(lista->nodo_inicio, posicion);
}

bool lista_contiene_clave(lista_t* lista, char* clave){
    if(!lista)
        return false;

    return (posicion_por_clave(lista, clave) != ERROR);
}

void* lista_primero(lista_t* lista){
    return (lista && lista->nodo_inicio) ? lista->nodo_inicio->elemento : NULL;
}

void* lista_ultimo(lista_t* lista){
    return (lista && lista->nodo_fin) ? lista->nodo_fin->elemento : NULL;
}

bool lista_vacia(lista_t* lista){
    return (lista) ? lista_tamanio(lista) == 0 : true;
}

size_t lista_tamanio(lista_t* lista){
    return (lista) ? lista->cantidad : 0;
}

/*
 * Pre: Recibira uan referencia a un nodo de la lista
 * Post: Destruira los nodos de la lista
 */
void destruir_nodo(nodo_t* nodo, void (*destructor)(void*)){
    if(nodo){
        destruir_nodo(nodo->siguiente,destructor);
        if(destructor)
            destructor(nodo->elemento);
        free(nodo);
    }
}

void lista_destruir(lista_t* lista){
    lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t* lista, void (*destructor)(void*)){
    if(lista){
        destruir_nodo(lista->nodo_inicio, destructor);
        free(lista);
    }
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista)
        return NULL;
    lista_iterador_t* iterador = calloc(1, sizeof(lista_iterador_t));
    
    iterador->corriente = lista->nodo_inicio;
    iterador->lista = lista;
    
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador)
        return false;

    return iterador->corriente;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(!iterador)
        return false;

    if(lista_iterador_tiene_siguiente(iterador))
        iterador->corriente = iterador->corriente->siguiente;

    return iterador->corriente;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    return (iterador && iterador->corriente ? iterador->corriente->elemento : NULL);
}

char* lista_iterador_clave_actual(lista_iterador_t* iterador){
    return (iterador && iterador->corriente ? iterador->corriente->clave : NULL);
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t nodo_con_cada_elemento(nodo_t* nodo, bool (*funcion)(void*, char*, void*), void *contexto, bool* seguir){

  if(!nodo || (*seguir))
    return 0;

  *seguir = funcion(nodo->elemento, nodo->clave, contexto);

  return 1+nodo_con_cada_elemento(nodo->siguiente, funcion, contexto, seguir);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, char*, void*), void *contexto, bool* seguir){

    if(!seguir){
        bool seguir_flag = false;
        seguir = &seguir_flag;
    }

    return (!lista || !funcion ? 0 : nodo_con_cada_elemento(lista->nodo_inicio, funcion, contexto, seguir));
}

size_t nodo_con_cada_elemento_h(hash_t* hash, nodo_t* nodo, bool (*funcion)(hash_t*, const char*, void*), void *contexto, bool* seguir){

  if(!nodo || (*seguir))
    return 0;

  *seguir = funcion(hash, (const char*)nodo->clave, contexto);

  return 1+nodo_con_cada_elemento_h(hash, nodo->siguiente, funcion, contexto, seguir);

}

size_t lista_con_cada_elemento_h(hash_t* hash, lista_t* lista, bool (*funcion)(hash_t*, const char*, void*), void *contexto, bool* seguir){

    if(!seguir){
        bool seguir_flag = false;
        seguir = &seguir_flag;
    }

    return (!lista || !funcion ? 0 : nodo_con_cada_elemento_h(hash, lista->nodo_inicio, funcion, contexto, seguir));
}





/*
 * Pre: Recibe un elemento y una lista auxiliar
 * Post: Guardara el elemento dentro de la lista
 */
bool guardar_elementos(void* elemento, char* clave, void* conjunto_auxiliar){
  conjunto_t* conjunto = (conjunto_t*)conjunto_auxiliar;

  conjunto->array[conjunto->posicion] = elemento;
  conjunto->claves[conjunto->posicion] = clave;

  conjunto->posicion++;
  return (conjunto->posicion >= conjunto->tamanio);
}


size_t lista_recorrer(lista_t* lista, void** array, char** claves, size_t posicion_inicial, size_t tamanio_array){

    conjunto_t conjunto = {array, claves, tamanio_array, posicion_inicial};

    return lista_con_cada_elemento(lista, guardar_elementos, &conjunto, NULL);


}
