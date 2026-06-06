#include <stdio.h>
#include <stdlib.h>
#include "gato.h"
#include "tdas/extra.h"
//#define N 3

void mostrar_menu_gato(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub: Gato");
    puts("=====================================================");

    puts("1) Jugar PvP");
    puts("2) Jugar PvE");
    puts("3) Volver al Menu Principal");
}

void mostrar_tablero_gato(char tablero[][3]){
    printf("\n");
    printf(" %c | %c | %c\n", tablero[0][0], tablero[0][1], tablero[0][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", tablero[1][0], tablero[1][1], tablero[1][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", tablero[2][0], tablero[2][1], tablero[2][2]);
}

int ganador(char jugador, char tablero[][3]){
    for(short i = 0; i < 3; i++){
        if(tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) return 1; //filas 
    }
    for(short i = 0; i < 3; i++){
        if(tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) return 1; //columnas
    }

    if(tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador) return 1; // diagonal 
    if(tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador) return 1; // contradiagonal

    return 0; //no hay ganador
}

int empate(char tablero[][3]){
    for(short i = 0; i < 3; i++){
        for(short k = 0; k < 3; k++){
            if(tablero[i][k] != 'X' && tablero[i][k] != 'O') return 0; // hay espacios disponibles 
        }
    }
    return 1; // no hay espacios disponible
}


void pvp(char tablero[][3]){
    int posicion;
    char jugador = 'X';

    while(1){
        mostrar_tablero_gato(tablero);

        printf("jugador: %c, elige una posicion:", jugador);
        scanf("%d", &posicion);

        if(posicion < 1 || posicion > 9){
            printf("Posicion no valida\n");
            continue;
        }

        int fila = (posicion - 1) / 3; // formula para pasar de un numero a una coord de matriz (fila)
        int columna = (posicion - 1) % 3; // formula para pasar de un numero a una coord de matriz (matriz)
        
        if(tablero[fila][columna] == 'X' || tablero[fila][columna] == 'O'){
            printf("casilla ocupada");
            continue;
        }

        tablero[fila][columna] = jugador; 

        if(ganador(jugador, tablero)){
            mostrar_tablero_gato(tablero);
            printf("jugador %c gana!\n", jugador);
            break;
        }
        if(empate(tablero)){
            mostrar_tablero_gato(tablero);
            printf("Empate!\n");
            break;
        }

        if(jugador == 'X') jugador = 'O';
        else jugador = 'X';
    }

}



void gato(){
    char opcion;
    char tablero[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };
    
    do
    {
        mostrar_menu_gato();
        printf("Ingrese una opcion");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            pvp(tablero);
            break;
        case '2':
            //pve();
            break;
        case '3':
            puts("Saliendo de gato");
            puts("Volviendo al menu principal");
            break;
        default:
            puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();
    }while(opcion != '3');

}