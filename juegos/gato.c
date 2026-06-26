#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gato.h"
#include "tdas/extra.h"
#include "tdas/list.h"
#include "ranking.h"

void mostrar_menu_gato(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub: Gato");
    puts("=====================================================");

    puts("1) Jugar PvP");
    puts("2) Jugar PvE");
    puts("3) Volver al Menu Principal");
}

void reiniciar_tablero(char tablero[][3]){
    char valor = '1';
    for(short i = 0; i < 3; i++){
        for(short k = 0; k < 3; k++){
            tablero[i][k] = valor++;
        }
    }
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

    return 0; //no hay ganador todavia
}

int empate(char tablero[][3]){
    //lo que se hace aqui es ver si hay espacios disponibles
    //porque si hay eso quiere decir que hay jugadas
    for(short i = 0; i < 3; i++){
        for(short k = 0; k < 3; k++){
            if(tablero[i][k] != 'X' && tablero[i][k] != 'O') return 0;  
        }
    }
    return 1; // no hay espacios disponible por lo tanto no hay ganador
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

        // evalua si es una casilla valida
        if(tablero[fila][columna] == 'X' || tablero[fila][columna] == 'O'){
            printf("casilla ocupada");
            continue;
        }
        // guarda la jugada en la matriz
        tablero[fila][columna] = jugador; 

        // resultados de la partida
        if(ganador(jugador, tablero)){
            mostrar_tablero_gato(tablero);
            printf("Jugador %c gana!\n", jugador);
            break;
        }
        if(empate(tablero)){
            mostrar_tablero_gato(tablero);
            printf("Empate!\n");
            break;
        }

        // cambia para que el siguiente turno se del otro jugador
        if(jugador == 'X') jugador = 'O';
        else jugador = 'X';
    }
}

void mostrar_menu_dificultades_gato(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Gato: PvE");
    puts("=====================================================");

    puts("1) Facil");
    puts("2) Normal");
    puts("3) Dificil"); 
    puts("4) Volver al Menu de Gato");
}

List *lista_adyacentes(char tablero[][3], char cpu){
    List *lista = list_create();

    for(short i = 0; i < 3; i++){
        for(short k = 0; k < 3; k ++){
            if(tablero[i][k] != 'X' && tablero[i][k] != 'O'){
                Coord *pos = (Coord *)malloc(sizeof(Coord));
                pos->x = i;
                pos->y = k;
                list_pushBack(lista, pos);
            }
        }
    }
    return lista;
}

// la struct Coord la deje en el gato.h
void movimiento_aleatorio(char tablero[][3], char cpu){

    List *estadosAdyacentes = lista_adyacentes(tablero, cpu);

    int talla = list_size(estadosAdyacentes);
    if(talla == 0) return;

    srand(time(NULL));
    int random = rand() % talla;
    Coord *pos = list_first(estadosAdyacentes);

    Coord *jugada = NULL;
    int contador = 0;
    while(pos != NULL){
        if(contador == random){
            jugada = pos;
            break;
        }
        pos = list_next(estadosAdyacentes);
        contador++;
    }

    if(jugada != NULL) tablero[jugada->x][jugada->y] = cpu;
}

int minimax(char tablero[][3], int esMaximizador, char cpu)
{
    char jugador = 'X'; 
    if(ganador(cpu, tablero)) return 10;
    if(ganador(jugador, tablero)) return -10; 
    if(empate(tablero)) return 0; 
    if(esMaximizador){
        int mejor = -1000;
        for(int i  = 0; i < 3; i++){
            for(int j = 0; j < 3; j ++){
                if(tablero[i][j] != 'X' && tablero[i][j] != 'O'){
                    char respaldo = tablero[i][j];
                    tablero[i][j] = cpu;
                    int valor = minimax(tablero, 0, cpu);
                    tablero[i][j] = respaldo; 
                    if(valor > mejor) mejor = valor;
                }
            }
        }
        return mejor;
    }
    else{
        int mejor = 1000; 

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(tablero[i][j] != 'X' && tablero[i][j] != 'O'){
                    char respaldo = tablero[i][j];
                    tablero[i][j] = jugador; 
                    int valor = minimax(tablero, 1, cpu);
                    tablero[i][j] = respaldo;
                    if(valor < mejor) mejor = valor;
                }
            }
        }
        return mejor;
    }
}

void mov_minimax(char tablero[][3], char cpu){
    int mejorValor = -1000;
    int mejorFila = -1;
    int mejorColumna = -1;

    for(int i = 0; i < 3; i ++){
        for(int j = 0; j < 3; j++){
            if(tablero[i][j] != 'X' && tablero[i][j] != 'O'){
                char respaldo = tablero[i][j]; 
                tablero[i][j] = cpu; 
                int valor = minimax(tablero, 0, cpu);
                tablero[i][j] = respaldo;

                if(valor > mejorValor){
                    mejorValor = valor; 
                    mejorFila = i;
                    mejorColumna = j;
                }
            }
        }
    }
    if(mejorFila != -1) tablero[mejorFila][mejorColumna] = cpu;
}

void pve(char tablero[][3], char dificultad, char cpu){
    int posicion;
    int random;
    int puntaje = 0;
    int puntaje_victoriaGT;
    int puntaje_empateGT;

    if (dificultad == '1') puntaje_victoriaGT = 250;
    else if (dificultad == '2') puntaje_victoriaGT = 500;
    else puntaje_victoriaGT = 1250;

    if (dificultad == '1') puntaje_empateGT = 100;
    if (dificultad == '2') puntaje_empateGT = 300;
    else puntaje_empateGT = 650;

    while(1){
        mostrar_tablero_gato(tablero);

        //turno de el jugador 
        printf("jugador X, elige una posicion:");
        scanf("%d", &posicion);

        if(posicion < 1 || posicion > 9){
            printf("Posicion no valida\n");
            continue;
        }

        int fila = (posicion - 1) / 3;
        int columna = (posicion - 1) % 3; 

        if(tablero[fila][columna] == 'X' || tablero[fila][columna] == 'O'){
            printf("casilla ocupada");
            continue;
        }

        tablero[fila][columna] = 'X';

        if(ganador('X', tablero)){
            mostrar_tablero_gato(tablero);
            printf("jugador X gana!\n");
            reiniciar_tablero(tablero);
            puntaje += puntaje_victoriaGT;
            printf("\n %d \n", puntaje);
            continue;
        }

        if(empate(tablero)){
            mostrar_tablero_gato(tablero);
            printf("Empate!\n");
            reiniciar_tablero(tablero);
            puntaje += puntaje_empateGT;
            printf("\n %d \n", puntaje);
            continue;
        }

        //turno del cpu
        printf("turno del cpu\n");

        //genera un numero y se le aplica el %100 para que este en un rang
        //para que este en un rango de 0 a 99 y por eso se le suma 1
        random = rand() % 100 + 1;

        switch(dificultad){
            case '1': //facil
                movimiento_aleatorio(tablero, cpu);
                break;
            case '2': //mormal
                if(random < 50) movimiento_aleatorio(tablero, cpu);
                else mov_minimax(tablero, cpu); // algoritmo para hacer mas dificil el cpu 
                break;
            case '3':  //dificil
                if(random < 10) movimiento_aleatorio(tablero, cpu);
                else mov_minimax(tablero, cpu);
                break;    
        }

        if(ganador(cpu, tablero)){
            mostrar_tablero_gato(tablero);
            printf("Cpu gana!\n");
            registrar_puntaje(JUEGA_GATO, puntaje);
            break;
        }

        if(empate(tablero)){
            mostrar_tablero_gato(tablero);
            printf("Empate!\n");
            reiniciar_tablero(tablero);
            puntaje += puntaje_empateGT;
            printf("\n %d \n", puntaje);
            continue;
        }
    }

}

void pve_menu(char tablero[][3]){
    char opcion;

    do
    {
        mostrar_menu_dificultades_gato();
        printf("Ingrese una dificultad: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':// facil
            reiniciar_tablero(tablero);
            pve(tablero, opcion, 'O');
            break; 
        case '2'://normal (50% de movimientos aleatorios)
            reiniciar_tablero(tablero);
            pve(tablero, opcion, 'O');
            break;
        case '3'://dificil (20% de movimientos aletorios)
            reiniciar_tablero(tablero);
            pve(tablero, opcion, 'O');
            break;
        case '4':
            printf("Saliendo de la opcion pve...");
            break; 
        default:
            puts("Opción de dificultad no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();

    }while(opcion != '4');
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
        printf("Ingrese una opcion de juego: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            reiniciar_tablero(tablero);
            pvp(tablero);
            break;
        case '2':
            reiniciar_tablero(tablero);
            pve_menu(tablero);
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