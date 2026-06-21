#ifndef conecta4_H
#define conecta4_H

#define filas 6
#define columnas 7

#define max_hijos 7
#define INF 10000000



typedef struct {
    char table[filas][columnas];
} juego;

typedef struct nodo nodo;

typedef struct nodo{
    juego estado;
    int columna_jugada;
    int puntaje;
    

    struct nodo *hijos[max_hijos];
    int cantidad_hijos;


} nodo;

typedef struct {
    char jugador;
    int columna;
} movimiento;


void reiniciar_tablero(juego *j);
void pvp(juego *j);
void pve(juego *j);
int jugada(juego *j,int posicion, char jugador);
int empate_c4(juego *j);
int ganaron_c4(juego *j, char simbolo);
void conecta4();

#endif