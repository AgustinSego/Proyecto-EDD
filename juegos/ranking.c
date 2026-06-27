#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/extra.h"

// Variables estaticas que serviran para almacenar las partidas de la sesion actual
static List *lista_gato = NULL;
static List *lista_conecta4 = NULL;
static List *lista_buscaminas = NULL;

// Se inicializan las listas que almacenaran los registros de puntajes obtenidos por los jugadores. Se llama solamente una vez al iniciar el programa 
void incializar_rankings(){
  lista_gato = list_create();
  lista_conecta4 = list_create();
  lista_buscaminas = list_create();
}

/* Recibe el puntaje final de una partida y lo guarda en la lista correspondiente.
Complejidad: O(1) ya que este es insertado directamente al final de la lista enlazada correspondiente al juego
El nombre del juego esta definido previamente para evitar que quien juegue ponga variantes del nombre del juego,
con esto nos evitamos hacer una funcion de "comparacion" contra variantes de una palabra (nombre del juego en este caso) */

void registrar_puntaje(const char *nombre_juego, int puntaje){
  char nombre[50];

  printf("\n¡Excelente partida! Ingrese su nombre para el ranking: ");
  scanf(" %49s", nombre);
  // Reserva de memoria para el  nuevo registro
  RecordRanking *record = (RecordRanking *)malloc(sizeof(RecordRanking));
  if (record == NULL) return;

  // Llenado de la estructura con los datos del jugador
  strcpy(record -> nombre, nombre);
  record -> puntaje = puntaje;

  //Clasificamos y guardamos el registro en la lista en memoria correspondiente 
  if (strcmp(nombre_juego, JUEGA_GATO) == 0){
    list_pushBack(lista_gato, record);
  }
  else if (strcmp(nombre_juego, JUEGA_CONECTA4) == 0){
    list_pushBack(lista_conecta4, record);
  }
  else if (strcmp(nombre_juego, JUEGA_BUSCAMINAS) == 0) {
        list_pushBack(lista_buscaminas, record);
    } 
    else {
        free(record);
        return;
    }
  printf("¡Puntaje %d fue almacenado en %s\n", puntaje, nombre_juego);
}

/* Extrae los datos de una lista, los ordena usando un Max-Heap y muestra los 3 nombres con mayor puntaje
Complejidad: O(n log n) por la insercion y extraccion en el monticulo*/

void procesar_y_mostrar_subranking(List *lista_juego, const char *titulo_juego){
  printf("\n>>> %s <<<\n", titulo_juego);
  // Validacion de seguridad: Para no trabajar con listas vacias
  if(lista_juego == NULL || list_size(lista_juego) == 0){
    puts(" [No hay datos]");
    return;
  }
  // Se crea un monticulo de maximos temporal para ordenar los puntajes de mayor a menor 
  Heap *temporal = heap_create();
  RecordRanking *actual = (RecordRanking *)list_first(lista_juego);

  // Insercion en el Heap
  // Complejidad: O(n log n)
  while(actual != NULL){
    // Se usa el puntaje como prioridad matematica para que el mayor suba a la raiz
    heap_push(temporal, actual, actual -> puntaje);
    actual = (RecordRanking *)list_next(lista_juego);
  }

  printf("  %-5s | %-20s | %-10s\n", "Pos", "Jugador", "Puntaje");
  printf("  ------------------------------------------\n");

  int posicion = 1;

  // Extraccion del Heap (hundimiento), los primeros 3 (3 mayores)
  // Complejidad: O(k log n), k = 3
  while (heap_top(temporal) != NULL && posicion <= 3) {
    RecordRanking *rec = (RecordRanking *)heap_top(temporal);

    printf("   %-5d | %-20s | %-10d\n", posicion, rec->nombre, rec->puntaje);
    // Se elimina la raiz, forzando una reestructuracion
    heap_pop(temporal);
    posicion++;
  }
    free(temporal); 
}

void mostrar_ranking(){
  limpiarPantalla();
  puts("======================================");
  puts("           TABLA DE POSICIONES        ");
  puts("======================================");

  procesar_y_mostrar_subranking(lista_gato, "GATO");
  procesar_y_mostrar_subranking(lista_conecta4, "CONECTA 4");
  procesar_y_mostrar_subranking(lista_buscaminas, "BUSCAMINAS");
  puts("======================================");
}

// Libera toda la memoria reservada dinamicamente antes de cerrar el programa
void liberar_ranking(){
  if (lista_gato != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_gato);
    while (rec != NULL) { 
      free(rec); 
      rec = (RecordRanking *)list_next(lista_gato); 
    }
    list_clean(lista_gato); free(lista_gato);
  }
  
  if (lista_conecta4 != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_conecta4);
    while (rec != NULL) { 
      free(rec); 
      rec = (RecordRanking *)list_next(lista_conecta4);
    }
    list_clean(lista_conecta4); 
    free(lista_conecta4);
  }
  
  if (lista_buscaminas != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_buscaminas);
    while (rec != NULL) { 
      free(rec); 
      rec = (RecordRanking *)list_next(lista_buscaminas); 
    }
    list_clean(lista_buscaminas); 
    free(lista_buscaminas);
  }
}
