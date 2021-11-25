#ifndef __LISTA_H__
#define __LISTA_H__

#include <stdbool.h>
#include <stddef.h>
#include "hash.h"


/**
 * Las siguientes estructuras deberían ser privadas y no está permitido el
 * acceso directo a los campos por fuera de la implementación.
 *
 * El motivo por el cual las estructuras son públicas es para asegurarnos de que
 * la implementación realizada es la pedida
 */
typedef struct nodo{
    void* elemento;
    char* clave;
    struct nodo* siguiente;
}nodo_t;

typedef struct lista{
    nodo_t* nodo_inicio;
    nodo_t* nodo_fin;
    size_t cantidad;
}lista_t;

typedef struct lista_iterador{
    nodo_t* corriente;
    lista_t* lista;
}lista_iterador_t;

/**
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t* lista_crear();

/**
 * Inserta un elemento al final de la lista.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t* lista_insertar(lista_t* lista, void* elemento, char* clave);


/**
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 *
 * Devuelve el 0 si lo elimino o -1 en caso de error.
 */
int lista_quitar(lista_t* lista);

/**
 * Quita de la lista el elemento que se encuentra la clave
 * indicada, donde 0 es el primer elemento de la lista.
 *
 * En caso de no existir esa clave o en caso de NULL se devuelve -1
 *
 * Devuelve el 0 si pudo eliminar el nodo.
 *
 */
int lista_quitar_por_clave(lista_t* lista, char* clave);

/**
 * Devuelve el elemento con la clave indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha clave devuelve NULL.
 */
void* lista_elemento_por_clave(lista_t* lista, char* clave);

/**
 *
 * Devuelve true si la lista contiene la clave indicada,
 *
 * Si no se contiene la clave se devuele false.
 *
 */
bool lista_contiene_clave(lista_t* lista, char* clave);





/**
 * Devuelve el primer elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_primero(lista_t* lista);

/**
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_ultimo(lista_t* lista);

/**
 * Devuelve true si la lista está vacía (o no existe) o false en caso contrario.
 */
bool lista_vacia(lista_t* lista);

/**
 * Devuelve la cantidad de elementos almacenados en la lista.
 * Una lista que no existe no puede tener elementos.
 */
size_t lista_tamanio(lista_t* lista);

/**
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t* lista);

/**
 * Destruye la lista liberando la memoria reservada por la misma.
 *
 * Adicionalmente invoca el destructor en cada uno de los elementos almacenados
 * en la lista (si la funcion destructor no es NULL).
 * 
 */
void lista_destruir_todo(lista_t* lista, void (*destructor)(void*));

/**
 * Crea un iterador para una lista. El iterador creado es válido desde
 * el momento de su creación hasta que no haya mas elementos por
 * recorrer o se modifique la lista iterada (agregando o quitando
 * elementos de la lista). 
 *
 * Al momento de la creación, el iterador queda listo para devolver el
 * primer elemento utilizando lista_iterador_elemento_actual.
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t* lista_iterador_crear(lista_t* lista);

/**
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador);

/**
 * Avanza el iterador al siguiente elemento.
 * Devuelve true si pudo avanzar el iterador o false en caso de
 * que no queden elementos o en caso de error.
 *
 * Una vez llegado al último elemento, si se invoca a
 * lista_iterador_elemento_actual, el resultado siempre será NULL.
 */
bool lista_iterador_avanzar(lista_iterador_t* iterador);

/**
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void* lista_iterador_elemento_actual(lista_iterador_t* iterador);

/**
 *
 * Devuelve la clave actual del iterador o NULL en caso de que no
 * exista dicha clave o en caso de error.
 *
 */
char* lista_iterador_clave_actual(lista_iterador_t* iterador);

/**
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t* iterador);

/**
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma como primer parámetro. Dicha función puede devolver true si se deben
 * seguir recorriendo elementos o false si se debe dejar de iterar elementos.
 *
 * El puntero contexto se pasa como segundo argumento a la función del usuario.
 *
 * La función devuelve la cantidad de elementos iterados o 0 en caso de error.
 */
size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, char*, void*), void *contexto, bool* seguir);



size_t lista_con_cada_elemento_h(hash_t* hash, lista_t* lista, bool (*funcion)(hash_t*, const char*, void*), void *contexto, bool* seguir);




/**
 * Recorre el arbol según el recorrido especificado y va almacenando los
 * elementos en el array hasta completar el recorrido o quedarse sin espacio en
 * el array.
 *
 * El array tiene un tamaño maximo especificado por tamanio_array.
 *
 * Devuelve la cantidad de elementos que fueron almacenados exitosamente en el
 * array.
 */
size_t lista_recorrer(lista_t* lista, void** array, char** claves, size_t posicion_inicial, size_t tamanio_array);


//CAMBIAR PRE Y POST DE LAS ULT FUNCIONES -> IGUALES A LAS DE HASH

#endif /* __LISTA_H__ */
