#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/extra.h"

static List *lista_gato = NULL;
static List *lista_conecta4 = NULL;
static List *lista_buscaminas = NULL;

void incializar_rankings(){
  lista_gato = list_create();
  lista_conecta4 = list_create();
  lista_buscaminas = list_create();
}

void registrar_puntaje(const char *nombre_juego, int puntaje){
  char nombre[50];

  printf("\n¡Excelente partida! Ingrese su nombre para el ranking: ");
  scanf(" %49s", nombre);
  RecordRanking *record = (RecordRanking *)malloc(sizeof(RecordRanking));
  if (record == NULL) return;

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

void procesar_y_mostrar_subranking(List *lista_juego, const char *titulo_juego){
  printf("\n>>> %s <<<\n", titulo_juego);

  if(lista_juego == NULL || list_size(lista_juego) == 0){
    puts(" [No hay datos]");
    return;
  }

  Heap *pq_temporal = heap_create();
  RecordRanking *actual = (RecordRanking *)list_first(lista_juego);
  while(actual != NULL){
    heap_push(pq_temporal, actual, actual -> puntaje);
    actual = (RecordRanking *)list_next(lista_juego);
  }

  printf("  %-5s | %-20s | %-10s\n", "Pos", "Jugador", "Puntaje");
  printf("  ------------------------------------------\n");

  int posicion = 1;
  while (heap_top(pq_temporal) != NULL) {
      RecordRanking *rec = (RecordRanking *)heap_top(pq_temporal);
      printf("   %-5d | %-20s | %-10d\n", posicion++, rec->nombre, rec->puntaje);
      heap_pop(pq_temporal);
  }

  free(pq_temporal);
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

void liberar_ranking(){
  if (lista_gato != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_gato);
    while (rec != NULL) { free(rec); rec = (RecordRanking *)list_next(lista_gato); }
    list_clean(lista_gato); free(lista_gato);
  }
  
  if (lista_conecta4 != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_conecta4);
    while (rec != NULL) { free(rec); rec = (RecordRanking *)list_next(lista_conecta4); }
    list_clean(lista_conecta4); free(lista_conecta4);
  }
  
  if (lista_buscaminas != NULL) {
    RecordRanking *rec = (RecordRanking *)list_first(lista_buscaminas);
    while (rec != NULL) { free(rec); rec = (RecordRanking *)list_next(lista_buscaminas); }
    list_clean(lista_buscaminas); free(lista_buscaminas);
  }
}