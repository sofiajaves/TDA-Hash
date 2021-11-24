#include "hash.h"
#include "abb.h"
#include <string.h>

#define EXITO 0
#define ERROR -1
#define CAPACIDAD_MINIMA 3
#define CAPACIDAD_MAXIMA_PORCENTUAL 75
#define PROMEDIO_ELEMENTOS 3
#define MULTIPLICADOR_REHASH 2

//funcion de hash
typedef size_t (*funcion_de_hash)(hash_t*, const char*);

struct hash{

    abb_t** tabla;
    size_t tamanio_tabla;
    funcion_de_hash hash;
    hash_destruir_dato_t destructor;
};

/*
 *
 */
size_t sumatoria_de_caracteres(const char* clave, size_t indice){

    if(indice > strlen(clave))
        return 0;

    return (clave[indice]+sumatoria_de_caracteres(clave, indice+1));
}

/*
 * Suma los caracteres del string, lo multiplica por la longitud de la clave y toma modulo con el tamanio de la tabla hash
 */
size_t funcion_calculo_de_hash(hash_t* hash, const char* clave){

    return((sumatoria_de_caracteres(clave,0)*strlen(clave))%(hash->tamanio_tabla));
}


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
    
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash)
        return NULL;

    hash->tamanio_tabla = (capacidad_inicial >= CAPACIDAD_MINIMA ? capacidad_inicial : CAPACIDAD_MINIMA);

    hash->tabla = calloc(hash->tamanio_tabla,sizeof(abb_t*));
    if(!hash->tabla){
        free(hash);
        return NULL;
    }

    hash->hash = funcion_calculo_de_hash;
    hash->destructor = destruir_elemento;

    return hash;
}

//ver si es necesario. Necesario es PERO capaz no conviene hacer una funcion aparte
abb_t* arbol_en_tabla(hash_t* hash, size_t posicion){

    if(!hash->tabla[posicion])
        hash->tabla[posicion] = abb_crear();

    return hash->tabla[posicion];
}

int crear_listas(void*** puntero_array, char*** puntero_claves, size_t tamanio){

    *puntero_array = calloc(tamanio,sizeof(void*));
    if(!*puntero_array)
        return ERROR;

    *puntero_claves = calloc(tamanio,sizeof(char*));
    if(!*puntero_claves){
        free(puntero_array);
        return ERROR;
    }

    return EXITO;
}

int guardar_elementos_en_listas(hash_t* hash, void** array, char** claves, size_t tamanio){

    size_t elementos_guardados = 0;

    for(int i = 0; i < hash->tamanio_tabla; i++)
        elementos_guardados += abb_recorrer(hash->tabla[i], PREORDEN, array, claves, elementos_guardados, tamanio);

    if(elementos_guardados != tamanio){
        free(array);
        free(claves);
        return ERROR;
    }

    return EXITO;
}

int reemplazar_tabla(hash_t* hash){

    for(int i = 0; i < hash->tamanio_tabla; i++)
        abb_destruir(hash->tabla[i]);

    free(hash->tabla);

    hash->tamanio_tabla *= MULTIPLICADOR_REHASH; 

    hash->tabla = calloc(hash->tamanio_tabla,sizeof(abb_t*));

    return (!hash->tabla ? ERROR : EXITO);
}

/*
 * NO MIRAR MUCHO 
 */
int rehashear(hash_t* hash){

    size_t tamanio = hash_cantidad(hash);

    void** array;

    char** claves;

    if(crear_listas(&array, &claves,tamanio) == ERROR)
        return ERROR;

    if(guardar_elementos_en_listas(hash, array, claves, tamanio) == ERROR)
        return ERROR;
   
    if(reemplazar_tabla(hash) == ERROR)
        return ERROR;

    int resultado = EXITO;

    for(int i = 0; i < tamanio && resultado == EXITO; i++)
        resultado = hash_insertar(hash, claves[i], array[i]);

    free(array);
    free(claves);

    return resultado;
}

bool necesita_rehash(hash_t* hash){

    int factor_de_carga = ((hash_cantidad(hash)*100)/(hash->tamanio_tabla*PROMEDIO_ELEMENTOS));//factor de carga

    return (factor_de_carga >= CAPACIDAD_MAXIMA_PORCENTUAL);
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){

    if(!hash || !clave)
        return ERROR;

    if(necesita_rehash(hash)){
        int resultado = rehashear(hash);
        if(resultado == ERROR)
            return ERROR;
    }

    size_t posicion = hash->hash(hash, clave);

    abb_t* arbol_a_insertar = arbol_en_tabla(hash, posicion);

    abb_t* arbol_auxiliar = abb_insertar(arbol_a_insertar, elemento, (char*)clave);
    if(!arbol_auxiliar)
        return ERROR;

    return EXITO;
}


int hash_quitar(hash_t* hash, const char* clave){

    if(!hash || !clave || !hash_contiene(hash, clave))
        return ERROR;

    void* elemento = hash_obtener(hash, clave);

    size_t valor_hash = hash->hash(hash, clave);

    int resultado = abb_quitar(hash->tabla[valor_hash],(char*)clave);
    if(resultado == ERROR)
        return ERROR;
    if(hash->destructor)
        hash->destructor(elemento);

    return EXITO;
}

void* hash_obtener(hash_t* hash, const char* clave){
    
    if(!hash || !clave)
        return NULL;

    size_t valor_hash = hash->hash(hash, clave);

    if(!hash->tabla[valor_hash])
        return NULL;//no hay arbol en esa posicion 

    return abb_buscar(hash->tabla[valor_hash],(char*)clave);
}


bool hash_contiene(hash_t* hash, const char* clave){

    if(!hash || !clave)
    return false;

    size_t valor_hash = hash->hash(hash, clave);

    if(!hash->tabla[valor_hash])
        return false;//no hay arbol en esa posicion 

    return abb_contiene(hash->tabla[valor_hash],(char*)clave);
    
    //devolvera true si existe el elemento buscado
}


size_t hash_cantidad(hash_t* hash){

    if(!hash)
        return 0;

    size_t cantidad = 0;

    for(int i = 0; i < hash->tamanio_tabla; i++)
        cantidad += abb_tamanio(hash->tabla[i]);

    return cantidad;
}


void hash_destruir(hash_t* hash){
    if(hash){
        for(int i = 0; i < hash->tamanio_tabla; i++)
            abb_destruir_todo(hash->tabla[i], hash->destructor);

        free(hash->tabla);
        free(hash);
    }
}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    

    return 0;
}