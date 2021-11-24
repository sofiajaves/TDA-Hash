#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define EXITO 0
#define ERROR -1

/*
 * Struct creado para guardar los elementos del arbol durante los recorridos
 */

typedef struct lista{
  void** array;
  char** claves;
  size_t tamanio;
  size_t posicion;
}lista_t;


int comparador_claves(void* elemento1, void* elemento2){
  char* clave1 = (char*)elemento1;
  char* clave2 = (char*)elemento2;

  return(strcmp(clave1,clave2));
}

abb_t* abb_crear(){
    
  abb_t* abb = calloc(1,sizeof(abb_t));
  if(!abb)
    return NULL;

  abb->comparador = comparador_claves;
  return abb;
}

/*
 * Pre: Recibira el elemento que se quiere agregar
 * Post: Creara un nodo para agregar al arbol
 */
nodo_abb_t* nodo_crear(void* elemento, char* clave){

  nodo_abb_t* nodo_nuevo = calloc(1,sizeof(nodo_abb_t));
  if(!nodo_nuevo)
    return NULL;

  nodo_nuevo->clave = clave;
  nodo_nuevo->elemento = elemento;
  return nodo_nuevo;
}

/*
 * Pre: Recibira una referencia a un nodo, un elemento, el comparador y un puntero a un resultado
 * Post: Insertara el nodo en el arbol. Si el nodo no pudiera ser insertado se cambiara el resultado a ERROR
 */
nodo_abb_t* nodo_insertar(nodo_abb_t* nodo, void* elemento, char* clave, abb_comparador comparador, int* resultado){

  if(!nodo){
    nodo_abb_t* nodo_nuevo = nodo_crear(elemento, clave);
    
    *resultado = (!nodo_nuevo ? ERROR : EXITO);
    return nodo_nuevo; 
  }

  int comparacion = comparador(clave,nodo->clave);

  if(comparacion == 0){
    nodo->elemento = elemento;
    *resultado = EXITO;
  }

  else if(comparacion < 0)
    nodo->izquierda = nodo_insertar(nodo->izquierda, elemento, clave, comparador, resultado);

  else
    nodo->derecha = nodo_insertar(nodo->derecha, elemento, clave, comparador, resultado);

  return nodo;
}

abb_t* abb_insertar(abb_t* arbol, void* elemento, char* clave){

  if(!arbol)
    return NULL;

  int resultado;

  bool tiene_clave = abb_buscar(arbol, clave);

  arbol->nodo_raiz = nodo_insertar(arbol->nodo_raiz, elemento, clave, arbol->comparador, &resultado);

  if(!tiene_clave)
    arbol->tamanio++;

  return (resultado == ERROR ? NULL : arbol);
}

/*
 * Pre: Recibira una referencia a un nodo y a su nodo predecesor
 * Post: Buscara el nodo que se encuentre mas a la derecha (en profundidad)
 */
nodo_abb_t* nodo_mas_derecha(nodo_abb_t* nodo, nodo_abb_t** nodo_predecesor){

  if(!nodo)
    return NULL;

  if(!nodo->derecha){
    *nodo_predecesor = nodo;
    return nodo->izquierda;
  }
  nodo->derecha = nodo_mas_derecha(nodo->derecha, nodo_predecesor);
  return nodo;
} 

/*
 * Pre: Recibe una referencia al nodo, un elemento y el comparador del arbol
 * Post: Eliminara el nodo predecesor inorden
 */
nodo_abb_t* eliminar_nodo(nodo_abb_t* nodo, abb_comparador comparador){

  nodo_abb_t* nodo_predecesor = NULL;
  nodo->izquierda = nodo_mas_derecha(nodo->izquierda, &nodo_predecesor);

  if(nodo_predecesor){
    nodo_predecesor->izquierda = nodo->izquierda;
    nodo_predecesor->derecha = nodo->derecha;
  } else 
    nodo_predecesor = nodo->derecha;
  
  free(nodo);
  return nodo_predecesor;
}

/*
 * Pre: Recibe una referencia al nodo, un elemento, el comparador del arbol y un booleano que indica la eliminacion del nodo
 * Post: Buscara el nodo a eliminar
 */
nodo_abb_t* nodo_buscar_para_eliminar(nodo_abb_t* nodo, char* clave, abb_comparador comparador, bool* se_elimino){

  if(!nodo){
    *se_elimino = false;
    return NULL;
  }

  int resultado = comparador(nodo->clave, clave);

  if(resultado == 0)
    return eliminar_nodo(nodo, comparador);

  else if(resultado > 0)
    nodo->izquierda = nodo_buscar_para_eliminar(nodo->izquierda, clave, comparador, se_elimino);

  else
    nodo->derecha = nodo_buscar_para_eliminar(nodo->derecha, clave, comparador, se_elimino);

  return nodo;
}

int abb_quitar(abb_t* arbol, char* clave){

  if(!arbol)
    return ERROR;

  bool se_elimino = true;

  arbol->nodo_raiz = nodo_buscar_para_eliminar(arbol->nodo_raiz, clave, arbol->comparador, &se_elimino);
  if(!se_elimino)
    return ERROR;

  arbol->tamanio--;
  return EXITO;
}

/*
 * Pre: Recibe una referencia al nodo, un elemento y el comparador del arbol
 * Post: Buscara un nodo que contenga el elemento buscado
 */
void* nodo_buscar(nodo_abb_t* nodo, char* clave, abb_comparador comparador){

  if(!nodo)
    return NULL;

  int resultado = comparador(nodo->clave, clave);

  if(resultado == 0) 
    return nodo->elemento;

  nodo_abb_t* nodo_resultado = (resultado > 0 ? nodo->izquierda : nodo->derecha);

  return nodo_buscar(nodo_resultado, clave, comparador);
}

void* abb_buscar(abb_t* arbol, char* clave){
  return ((!arbol || abb_vacio(arbol)) ? NULL : nodo_buscar(arbol->nodo_raiz, clave, arbol->comparador));
}

bool abb_vacio(abb_t* arbol){
  return (!arbol ? true : abb_tamanio(arbol) == 0);
}

bool nodo_contiene(nodo_abb_t* nodo, char* clave, abb_comparador comparador){
  
  if(!nodo)
    return false;

  int resultado = comparador(nodo->clave, clave);

  if(resultado == 0) 
    return true;

  nodo_abb_t* nodo_resultado = (resultado > 0 ? nodo->izquierda : nodo->derecha);

  return nodo_contiene(nodo_resultado, clave, comparador);
}

bool abb_contiene(abb_t* arbol, char* clave){
  return (!arbol || abb_vacio(arbol)) ? false : nodo_contiene(arbol->nodo_raiz, clave, arbol->comparador);

}

size_t abb_tamanio(abb_t *arbol){
  return (!arbol ? 0 : arbol->tamanio);
}

/*
 * Pre: Recibe una referencia a un nodo y a la funcion destructora
 * Post: Destruira el nodo y los subarboles de este con todos los elementos si hay destructor
 */
void nodo_destruir(nodo_abb_t* nodo, void (*destructor)(void*)){

  if(nodo){
    nodo_destruir(nodo->izquierda, destructor);
    nodo_destruir(nodo->derecha, destructor);

    if(destructor)
      destructor(nodo->elemento);

    free(nodo);
  }
}

void abb_destruir(abb_t *arbol){
  abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)){

  if(arbol){
    nodo_destruir(arbol->nodo_raiz, destructor);
    free(arbol);
  }
}

/*
 * Pre: Recibe una referencia a un nodo, una funcion booleana, un puntero auxiliar que se envia como segundo parametro en la funcion booleana y una referencia a un booleano seguir
 * Post: Recorrera el arbol con la forma preorden. Si no pudiera hacerlo el bool seguir cambiara a false y se corta el recorrido. Devuelve la cantidad de iteraciones que hizo
 */
size_t recorrido_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, char*, void*), void* aux, bool* seguir){

  if(!nodo || !(*seguir))
    return 0;

  *seguir = funcion(nodo->elemento, nodo->clave, aux);

  size_t recorrido = recorrido_preorden(nodo->izquierda, funcion, aux, seguir);
  recorrido += recorrido_preorden(nodo->derecha, funcion, aux, seguir);

  return recorrido+1;
}

/*
 * Pre: Recibe una referencia a un nodo, una funcion booleana, un puntero auxiliar que se envia como segundo parametro en la funcion booleana y una referencia a un booleano seguir
 * Post: Recorrera el arbol con la forma postorden. Si no pudiera hacerlo el bool seguir cambiara a false y se corta el recorrido. Devuelve la cantidad de iteraciones que hizo
 */
size_t recorrido_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, char*, void*), void* aux, bool* seguir){
  if(!nodo || !(*seguir))
    return 0;

  size_t recorrido = recorrido_postorden(nodo->izquierda, funcion, aux, seguir);
  recorrido += recorrido_postorden(nodo->derecha, funcion, aux, seguir);

  if((*seguir)){
    *seguir = funcion(nodo->elemento, nodo->clave, aux);
    recorrido++;
  }

  return recorrido;
}

/*
 * Pre: Recibe una referencia a un nodo, una funcion booleana, un puntero auxiliar que se envia como segundo parametro en la funcion booleana y una referencia a un booleano seguir
 * Post: Recorrera el arbol con la forma inorden. Si no pudiera hacerlo el bool seguir cambiara a false y se corta el recorrido. Devuelve la cantidad de iteraciones que hizo
 */
size_t recorrido_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, char*, void*), void* aux, bool* seguir){
  if(!nodo || !(*seguir))
    return 0;

  size_t recorrido = recorrido_inorden(nodo->izquierda, funcion, aux, seguir);

  if((*seguir)){
    *seguir = funcion(nodo->elemento, nodo->clave, aux);
    recorrido++;
  }

  recorrido += recorrido_inorden(nodo->derecha, funcion, aux, seguir);
  return recorrido;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, char*, void *), void *aux){
  if(!arbol || !funcion)
    return 0;

  bool seguir = true;
  size_t contador = 0;

  switch(recorrido){
    case PREORDEN: 
      contador = recorrido_preorden(arbol->nodo_raiz, funcion, aux, &seguir);
      break;

    case POSTORDEN:
      contador = recorrido_postorden(arbol->nodo_raiz, funcion, aux, &seguir);
      break;

    case INORDEN:
      contador = recorrido_inorden(arbol->nodo_raiz, funcion, aux, &seguir);
      break;
  }
  return contador;
}

/*
 * Pre: Recibe un elemento y una lista auxiliar
 * Post: Guardara el elemento dentro de la lista
 */
bool guardar_elementos(void* elemento, char* clave, void* lista_auxiliar){
  lista_t* lista = (lista_t*)lista_auxiliar;

  lista->array[lista->posicion] = elemento;
  lista->claves[lista->posicion] = clave;

  lista->posicion++;
  return (lista->posicion < lista->tamanio);
}

size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, char** claves, size_t posicion_inicial, size_t tamanio_array){

  lista_t lista = {array, claves, tamanio_array, posicion_inicial};

  return abb_con_cada_elemento(arbol, recorrido, guardar_elementos, &lista);
}
