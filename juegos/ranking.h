#ifndef RANKING_H
#define RANKING_H

#define JUEGA_GATO "Gato"
#define JUEGA_CONECTA4 "Conecta 4"
#define JUEGA_BUSCAMINAS "Buscaminas"

typedef struct{
  char nombre[50];
  int puntaje;
} RecordRanking;

// Inicializa las estructuras de datos en memoria RAM
void incializar_rankings();

// Pide el nombre al jugador y guarda el registro en la lista correspondiente
void registrar_puntaje(const char *nombre_juego, int puntaje);

// Pasa los datos a Heaps temporales para ordenar y mostrar los 3 rankings simultaneamente
void mostrar_ranking();

// Libera la memoria dinamica antes de cerrar el programa
void liberar_ranking();

#endif /* RANKING_H */