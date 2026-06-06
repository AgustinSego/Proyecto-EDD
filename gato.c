#include <stdio.h>
#include <stdlib.h>
#include "gato.h"
#include "tdas/extra.h"
#define N = 3

void mostrar_menu(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub: Gato");
    puts("=====================================================");

    puts("1) Jugar PvP");
    puts("2) Jugar PvE");
    puts("3) Salir");
}

void pvp(int tablero[][N]){
    

}



void gato(){
    char opcion;
    int tablero[N][N] = {0};
    
    do
    {
        mostrar_menu();

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
    }while(opcion != '3')

}