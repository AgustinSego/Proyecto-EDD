#ifndef conecta4_H
#define conecta4_H

#define filas 6
#define columnas 7

typedef struct {
    char table[filas][columnas];
} juego;

void reiniciar_tablero(juego *j);
void pvp(juego *j);
void pve(juego *j);
int jugada(juego *j,int posicion, char jugador);
int empate_c4(juego *j);
int ganaron_c4(juego *j, char simbolo);
void conecta4();

#endif