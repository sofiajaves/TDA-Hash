#include "pa2mm.h"
#include "src/hash.h"

#define EXITO 0
#define ERROR -1

void dadaUnaTablaDeHash_seCreaExitosamente(){

    hash_t* hash = hash_crear(NULL, 3);

    pa2m_afirmar(hash != NULL, "Se creo el hash exitosamente");
    pa2m_afirmar(hash_cantidad(hash) == 0, "Al crear un hash la cantidad de elementos almacenados es 0");

    hash_destruir(hash);
}

void dadaUnaTablaDeHashYUnaCapacidadInicialMenorATres_seCreaLaTablaConCapacidadTres(){

    hash_t* hash = hash_crear(NULL, 2);

    pa2m_afirmar(hash != NULL, "Se creo el hash exitosamente");
    pa2m_afirmar(hash_cantidad(hash) == 0, "Al crear un hash la cantidad de elementos almacenados es 0");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_Y_seInsertaUnaClaveExitosamente(){

    hash_t* hash = hash_crear(NULL, 2);

    char* clave = "Hola";

    int elemento = 2;

    pa2m_afirmar(hash_insertar(hash, clave, &elemento)==EXITO, "Se inserto el elemento correctamente");
    pa2m_afirmar(hash_cantidad(hash) == 1, "Luego de insertar un elemento y su clave, la cantidad del hash es 1");
    pa2m_afirmar(hash_obtener(hash, clave) == &elemento, "Se obtiene el elemento correcto luego de insertarlo");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_seInsertanVariosElementos(){

    hash_t* hash = hash_crear(NULL, 3);

    char* clave = "Hola";
    char* clave1 = "Como";
    char* clave2 = "Estas";

    int elemento = 2;
    int elemento1 = 1;
    int elemento2 = 3;

    pa2m_afirmar(hash_insertar(hash, clave, &elemento)==EXITO, "Se inserto el elemento correctamente");
    pa2m_afirmar(hash_insertar(hash, clave1, &elemento1)==EXITO, "Se inserto el elemento correctamente");
    pa2m_afirmar(hash_insertar(hash, clave2, &elemento2)==EXITO, "Se inserto el elemento correctamente");

    pa2m_afirmar(hash_cantidad(hash) == 3, "Luego de insertar varios elementos y sus claves, la cantidad del hash es 3");

    pa2m_afirmar(hash_obtener(hash, clave) == &elemento, "Se obtiene el elemento correcto luego de insertarlo");
    pa2m_afirmar(hash_obtener(hash, clave1) == &elemento1, "Se obtiene el elemento correcto luego de insertarlo");
    pa2m_afirmar(hash_obtener(hash, clave2) == &elemento2, "Se obtiene el elemento correcto luego de insertarlo");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_Y_UnaClaveNula_NoSeInserta(){

    hash_t* hash = hash_crear(NULL, 2);

    char* clave = NULL;

    int elemento = 2;

    pa2m_afirmar(hash_insertar(hash, clave, &elemento)== ERROR, "No se puede insertar un elemento con clave NULL");
    pa2m_afirmar(hash_cantidad(hash) == 0, "No se pudo insertar el elemento por su clave NULL, la cantidad permanece en 0");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_seInsertanClavesHastaRehashear(){

    hash_t* hash = hash_crear(NULL, 3);

    char* clave = "Hola";
    char* clave1 = "Como";
    char* clave2 = "Estas";
    char* clave3 = "???";
    char* clave4 = "Me";
    char* clave5 = "Gusta";
    char* clave6 = "El";
    char* clave7 = "Arte";

    int elemento = 2;
    int elemento1 = 1;
    int elemento2 = 3;
    int elemento3 = 4;

    bool resultado = true;

    resultado &= hash_insertar(hash, clave, &elemento)==EXITO;
    resultado &= hash_insertar(hash, clave1, &elemento1)==EXITO;
    resultado &= hash_insertar(hash, clave2, &elemento2)==EXITO;
    resultado &= hash_insertar(hash, clave3, &elemento3)==EXITO;
    resultado &= hash_insertar(hash, clave4, &elemento1)==EXITO;
    resultado &= hash_insertar(hash, clave5, &elemento2)==EXITO;
    resultado &= hash_insertar(hash, clave6, &elemento3)==EXITO;
    resultado &= hash_insertar(hash, clave7, &elemento1)==EXITO;

    pa2m_afirmar(resultado, "Se insertaron todos los elementos correspondientes");

    pa2m_afirmar(hash_cantidad(hash) == 8, "Luego de insertar varios elementos y sus claves, teniendo en cuenta el factor de carga, se pudo insertar otra clave y la cantidad es 8");

    resultado = true;

    resultado &= hash_obtener(hash, clave) == &elemento; 
    resultado &= hash_obtener(hash, clave1) == &elemento1;
    resultado &= hash_obtener(hash, clave2) == &elemento2;
    resultado &= hash_obtener(hash, clave3) == &elemento3;
    resultado &= hash_obtener(hash, clave4) == &elemento1;
    resultado &= hash_obtener(hash, clave5) == &elemento2;
    resultado &= hash_obtener(hash, clave6) == &elemento3;
    resultado &= hash_obtener(hash, clave7) == &elemento1;

    pa2m_afirmar(resultado, "Luego de insertar varios elementos, se obtienen todos los correspondientes");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_Y_UnaClaveNoInsertada_seDevuelveNULLAlQuererObtenerla(){

    hash_t* hash = hash_crear(NULL, 2);

    char* clave1 = "Milanesa";

    pa2m_afirmar(hash_obtener(hash, clave1) == NULL, "Al querer obtener una clave no insertada, se obtiene un NULL");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_seInsertaUnaClaveYaExistente_Y_NoAumentaElTamanio(){

    hash_t* hash = hash_crear(NULL, 2);

    char* clave = "Milanesa";

    int elemento = 2;
    int elemento1 = 3;

    pa2m_afirmar(hash_insertar(hash, clave, &elemento)==EXITO, "Se inserto el elemento correctamente");
    pa2m_afirmar(hash_cantidad(hash) == 1, "Luego de insertar un elemento y su clave, la cantidad del hash es 1");
    pa2m_afirmar(hash_obtener(hash, clave) == &elemento, "Se obtiene el elemento correcto luego de insertarlo");

    pa2m_afirmar(hash_insertar(hash, clave, &elemento1)==EXITO, "Se actualizo el elemento con la misma clave");
    pa2m_afirmar(hash_cantidad(hash) == 1, "Luego de insertar un elemento con clave repetida, la cantidad del hash permanece y es 1");
    pa2m_afirmar(hash_obtener(hash, clave) == &elemento1, "Al insertar un elemento con clave repetida, se actualiza el elemento");

    hash_destruir(hash);

}

void dadaUnaTablaDeHashConVariosElementos_seEliminaUnElementoExitosamente(){

    hash_t* hash = hash_crear(NULL, 3);

    char* clave = "Milanesa";
    char* clave1 = "Sanguchito";
    char* clave2 = "Helado";

    int elemento = 2;
    int elemento1 = 1;
    int elemento2 = 3;

    hash_insertar(hash, clave, &elemento);
    hash_insertar(hash, clave1, &elemento1);
    hash_insertar(hash, clave2, &elemento2);

    pa2m_afirmar(hash_cantidad(hash) == 3, "Antes de quitar un elemento, la cantidad del hash es 3");

    pa2m_afirmar(hash_quitar(hash, clave2) == EXITO, "Se pudo eliminar el elemento correctamente");

    pa2m_afirmar(hash_cantidad(hash) == 2, "Luego de quitar un elemento, la cantidad del hash disminuye y es 2");

    hash_destruir(hash);
}

void dadaUnaTablaDeHashConVariosElementos_seEliminaUnElementoNuloExitosamente(){

    hash_t* hash = hash_crear(NULL, 3);

    char* clave = "Milanesa";
    char* clave1 = "Sanguchito";
    char* clave2 = "Helado";

    int elemento = 2;
    int elemento1 = 1;

    hash_insertar(hash, clave, &elemento);
    hash_insertar(hash, clave1, &elemento1);
    hash_insertar(hash, clave2, NULL);

    pa2m_afirmar(hash_cantidad(hash) == 3, "Antes de quitar un elemento que es NULL, la cantidad del hash es 3");

    pa2m_afirmar(hash_quitar(hash, clave2) == EXITO, "Se pudo eliminar el elemento NULL correctamente");

    pa2m_afirmar(hash_cantidad(hash) == 2, "Luego de quitar un elemento NULL, la cantidad del hash disminuye y es 2");

    hash_destruir(hash);
}

void dadaUnaTablaDeHash_seIntentaEliminarUnElementoNoInsertado(){

    hash_t* hash = hash_crear(NULL, 2);

    char* clave1 = "Milanesa";

    pa2m_afirmar(hash_quitar(hash, clave1) == ERROR, "Al querer eliminar una clave no insertada, se obtiene ERROR");

    hash_destruir(hash);
}


int main(){
    pa2m_nuevo_grupo("Pruebas de Hash");
    pa2m_nuevo_grupo("Pruebas de Creacion");
    dadaUnaTablaDeHash_seCreaExitosamente();
    printf("\n");
    dadaUnaTablaDeHashYUnaCapacidadInicialMenorATres_seCreaLaTablaConCapacidadTres();
    printf("\n");

    pa2m_nuevo_grupo("Pruebas de Insercion");
    dadaUnaTablaDeHash_Y_seInsertaUnaClaveExitosamente();
    printf("\n");
    dadaUnaTablaDeHash_seInsertanVariosElementos();
    printf("\n");
    dadaUnaTablaDeHash_Y_UnaClaveNula_NoSeInserta();
    printf("\n");
    dadaUnaTablaDeHash_seInsertanClavesHastaRehashear();
    printf("\n");
    dadaUnaTablaDeHash_Y_UnaClaveNoInsertada_seDevuelveNULLAlQuererObtenerla();
    printf("\n");
    dadaUnaTablaDeHash_seInsertaUnaClaveYaExistente_Y_NoAumentaElTamanio();
    printf("\n");

    pa2m_nuevo_grupo("Pruebas de Eliminacion");
    dadaUnaTablaDeHashConVariosElementos_seEliminaUnElementoExitosamente();
    printf("\n");
    dadaUnaTablaDeHashConVariosElementos_seEliminaUnElementoNuloExitosamente();
    printf("\n");
    dadaUnaTablaDeHash_seIntentaEliminarUnElementoNoInsertado();
    printf("\n");

    pa2m_nuevo_grupo("Pruebas de Hash con cada Elemento");

    printf("\n");


    printf("\n");
    pa2m_nuevo_grupo("Pruebas de Destruccion");



    //SOLO SI TENEMOS TIEMPO Y NO SOMOS VAGOS -> prueba de destructor no nulo. Prueba de destruccion con cada elemento.
    
    return pa2m_mostrar_reporte();
}