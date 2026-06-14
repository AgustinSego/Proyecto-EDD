#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "conecta4.h"
#define P1 'A'
#define P2 'B'
#define filas 6
#define columnas 7
void pintar(juego *j){
  for(short q = 0; q < columnas; q++){
      printf(" %d", q +1);
  }
  printf("\n");
  for(short i = 0; i < filas; i++){
      for(short k = 0; k < columnas; k++){
          printf(" %c", j->table[i][k]);
      }
    printf("\n");
  }
}
void reiniciar_tablero_c4(juego *j){
    for(short i = 0; i < filas; i++){
        for(short k = 0; k < columnas; k++){
            j->table[i][k] = '.';
        }
    }
}
int jugada(juego *j,int columna,char simbolo){
        for(int i = filas-1; i  >= 0; i--){
            if(j->table[i][columna] == '.'){
                j->table[i][columna] = simbolo;
                return 1;
            }
        }
    printf("la columna ya esta ocupada\n");
    return 0;
}
int empate_c4(juego *j){
    for(short i = 0; i < filas; i++){
        for(short k = 0; k < columnas; k++){
            if(j->table[i][k] == '.') return 0;
        }
    }
    return 1;
}
int ganaron_c4(juego *j, char simbolo){
    for(int i = 0; i < filas; i++){
        for(int k = 0; k <= columnas - 4; k++){
            if(j->table[i][k] == simbolo &&
               j->table[i][k+1] == simbolo &&
               j->table[i][k+2] == simbolo &&
               j->table[i][k+3] == simbolo){
                return 1;
            }
        }
    }
    for(int i = 0; i <= filas - 4; i++){
        for(int k = 0; k < columnas; k++){
            if(j->table[i][k] == simbolo &&
               j->table[i+1][k] == simbolo &&
               j->table[i+2][k] == simbolo &&
               j->table[i+3][k] == simbolo){
                return 1;
            }
        }
    }
    for(int i = 0; i <= filas - 4; i++){
        for(int k = 0; k <= columnas - 4; k++){
            if(j->table[i][k] == simbolo &&
               j->table[i+1][k+1] == simbolo &&
               j->table[i+2][k+2] == simbolo &&
               j->table[i+3][k+3] == simbolo){
                return 1;
            }
        }
    }
    for(int i = 0; i <= filas - 4; i++){
        for(int k = 3; k < columnas; k++){
            if(j->table[i][k] == simbolo &&
               j->table[i+1][k-1] == simbolo &&
               j->table[i+2][k-2] == simbolo &&
               j->table[i+3][k-3] == simbolo){
                return 1;
            }
        }
    }

    return 0;
}
void pvp_c4(juego *j){
    int posicion;
    char simbolo = P1;

    while (1){
        pintar(j);
        printf("jugador %c la columna que desea", simbolo);
        scanf("%d", &posicion);

        if (posicion < 0 || posicion > columnas){
            printf("columna invalida\n");
            continue;
        }
        int columna = posicion -1;
        if(!(jugada(j,columna, simbolo))){
            continue;
        }
        if(empate_c4(j)){
            printf("EMPATARON\n");
            break;
        }
        if(ganaron_c4(j,simbolo)){
            pintar(j);
            printf("el jugador %c gano\n", simbolo);
            break;
                
        }

        if(simbolo == P1) simbolo = P2;
        else simbolo = P1;
    }

        

}
void mostrar_menu_conecta_c4(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub: conecta4");
    puts("=====================================================");

    puts("1) Jugar PvP");
    puts("2) Jugar PvE");
    puts("3) Volver al Menu Principal");
}
void conecta4(){
    char opcion;
    juego *tablero = malloc(sizeof(juego));
    if (tablero == NULL){
      printf("error");
      return;
    }
  

    do
    {
          mostrar_menu_conecta_c4();
        printf("Ingrese una opcion de juego: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            reiniciar_tablero_c4(tablero);
            pvp_c4(tablero);
            break;
        case '2':
            reiniciar_tablero_c4(tablero);
            //pve_menu(tablero);
            break;
        case '3':
            puts("Saliendo de conecta4");
            puts("Volviendo al menu principal");
            break;
        default:
            puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();
    }while(opcion != '3');

}