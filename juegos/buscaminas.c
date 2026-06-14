#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tdas/extra.h"
#include "tdas/list.h"

// BM = buscaminas.

typedef struct
{
    bool es_mina;
    bool revelada;
    bool bandera;
    int minas_alrededor;
}CasillaBM;

typedef struct {
    int filas;
    int columnas;
    int num_minas;
    CasillaBM **matriz;
} TableroBM;

typedef struct
{
    int x;
    int y;
} Coord;


void mostrar_menuBM()
{
    puts("===================================");
    puts("      BIENVENIDO A BUSCAMINAS");
    puts("===================================");
    puts("1) Dificultad Facil: 8 x 8, 10 minas.");
    puts("2) Dificultad Moderada: 16 x 16, 40 minas.");
    puts("3) Dificultad Dificil: 24 x 24, 99 minas.");
    puts("4) Volver al menu principal.");
}

void mostrar_tableroBM(TableroBM* tablero) 
{
    // Coordenadas del eje x
    printf("     ");
    for (int col = 0; col < tablero->columnas; col++) 
    {
        printf("%2d ", col + 1); 
    }
    printf("\n");

    // tapa superior matriz
    printf("     ");
    for (int col = 0; col < tablero->columnas; col++) 
    {
        printf("---");
    }
    printf("\n");

    // Coordenadas del eje y)
    for (int fila = 0; fila < tablero->filas; fila++) 
    {
        printf("%2d  | ", fila + 1); 

        // relleno visual de la matriz
        for (int col = 0; col < tablero->columnas; col++) 
        {
            CasillaBM casilla = tablero->matriz[fila][col];

            if (casilla.revelada == true) 
            {
                if (casilla.es_mina == true)
                {
                    printf("@ "); // @ para las minas. 
                }  
                else 
                {
                    printf("%d  ", casilla.minas_alrededor); 
                }
            }
            else if (casilla.bandera == true) 
            {
                printf("B  ");
            } 
            else 
            {
                printf("#  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}


TableroBM* crear_tablero_buscaminas(int filas, int columnas, int num_minas)
{
    // se reserva memoria para el tablero
    TableroBM* tablero = (TableroBM*)malloc(sizeof(TableroBM));
    tablero->filas = filas;
    tablero->columnas = columnas;
    tablero->num_minas = num_minas;

    // se reserva memoria para la matriz del tablero.
    tablero->matriz = (CasillaBM**)malloc(filas * sizeof(CasillaBM*));
    for (int i = 0; i < filas ; i++)
    {
        tablero->matriz[i] = (CasillaBM*)malloc(columnas * sizeof(CasillaBM));
        for (int j = 0; j < columnas; j++)
        {
            tablero->matriz[i][j].es_mina = false;
            tablero->matriz[i][j].revelada = false;
            tablero->matriz[i][j].bandera = false;
            tablero->matriz[i][j].minas_alrededor = 0;
        }
    }

    return tablero;
}

void poner_minas(TableroBM* tablero)
{
    int cont_minas = 0;

    srand(time(NULL));
    while (cont_minas < tablero->num_minas)
    {
        // rand() funciona de forma que devuelve un numero aleatorio.
        int filaAzar = rand() % tablero->filas; // modulo para que se mantenga dentro del tablero.
        int columnaAzar = rand() % tablero->columnas;

        if (tablero->matriz[filaAzar][columnaAzar].es_mina == false)
        {
            tablero->matriz[filaAzar][columnaAzar].es_mina = true;
            cont_minas++;
        }
    }    
}

void valor_minas_adyacentes(TableroBM* tablero)
{
    // posiciones dentro del tablero
    for(int pos_f = 0; pos_f < tablero->filas ;pos_f++)
    {
        for(int pos_c = 0; pos_c < tablero->columnas ; pos_c++)
        {
            if (tablero->matriz[pos_f][pos_c].es_mina == true) continue;

            int contador = 0;

            // recorrido 3 x 3
            for (int rec_f = -1; rec_f <= 1 ; rec_f++)
            {
                for (int rec_c = -1; rec_c <= 1 ; rec_c++)
                {
                    int vecino_fil = pos_f + rec_f;
                    int vecino_col = pos_c + rec_c;

                    // verificar que se encuentre dentro del tablero
                    if ((vecino_fil >= 0) && (vecino_fil < tablero->filas) && (vecino_col >= 0) && (vecino_col < tablero->columnas))
                    {
                        if (tablero->matriz[vecino_fil][vecino_col].es_mina == true) contador++;
                    }
                }
            }
            tablero->matriz[pos_f][pos_c].minas_alrededor = contador;
        }
    }
}

void liberar_memoriaBM(TableroBM* tablero)
{
    //recorre las filas y libera las columnas dentro de estas.
    for (int i = 0; i < tablero->filas ; i++) free(tablero->matriz[i]);
    free(tablero->matriz);
    free(tablero);
}

void bfs(TableroBM* tablero, int pos_f, int pos_c)
{
    Coord* pos_i = (Coord*)malloc(sizeof(Coord));
    pos_i->x = pos_f;
    pos_i->y = pos_c;

    List* cola = list_create();
    list_pushBack(cola, pos_i);

    tablero->matriz[pos_f][pos_c].revelada = true;

    while(list_size(cola) > 0)
    {
        Coord* actual = (Coord*)list_popFront(cola);; 

        if (tablero->matriz[actual->x][actual->y].minas_alrededor > 0) continue;

        for (int rec_f = -1; rec_f <= 1; rec_f++) 
        {
            for (int rec_c = -1; rec_c <= 1; rec_c++) 
            {
                int vecino_fil = actual->x + rec_f;
                int vecino_col = actual->y + rec_c;

                if (vecino_fil >= 0 && vecino_fil < tablero->filas && vecino_col >= 0 && vecino_col < tablero->columnas) 
                {
                    if (tablero->matriz[vecino_fil][vecino_col].revelada == false && tablero->matriz[vecino_fil][vecino_col].es_mina == false) 
                    {
                        tablero->matriz[vecino_fil][vecino_col].revelada = true;
                        
                        if (tablero->matriz[vecino_fil][vecino_col].minas_alrededor == 0) 
                        {
                            Coord* vecino = (Coord*)malloc(sizeof(Coord));
                            vecino->x = vecino_fil;
                            vecino->y = vecino_col;
                            list_pushBack(cola, vecino);
                        }
                    }
                }
            }
        }
    }
    free(cola);
}

bool condicion_victoriaBM(TableroBM* tablero)
{
    for(int fila = 0; fila < tablero->filas ; fila++)
    {
        for (int columna = 0; columna < tablero->columnas; columna++)
        {
            // si hay una casilla que no es bomba y no ha sido revelada entonces aun no se gana.
            if (tablero->matriz[fila][columna].es_mina == false && tablero->matriz[fila][columna].revelada == false) return false;
        }
    }
    return true;
}

void jugarBM(TableroBM* tablero)
{
    int fila;
    int columna;
    char opcion;

    poner_minas(tablero);
    valor_minas_adyacentes(tablero);

    while(true)
    {
        mostrar_tableroBM(tablero);
        puts("Movimientos: [E] Revelar Casilla o Poner/Quitar Bandera [F]");
        puts("Ejemplo -> 7 7 E / (FILA, COLUMNA, MOVIMIENTO).");

        scanf("%d %d %c", &fila, &columna, &opcion);

        // se le resta 1 a cada parametro porque si no se desfasa la posición.
        fila--;
        columna--;

        if (fila < 0 || fila >= tablero->filas || columna < 0 || columna >= tablero->columnas) 
        {
            puts("Casilla no valida.");
            continue;
        }

        if (opcion == 'F' || opcion == 'f') 
        {
            if (tablero->matriz[fila][columna].revelada == true) 
            {
                puts("No puedes poner bandera en una casilla ya revelada.");
            }
            else 
            {
                if (tablero->matriz[fila][columna].bandera == true) tablero->matriz[fila][columna].bandera = false;
                else tablero->matriz[fila][columna].bandera = true;
            }
            continue;
        }

        if (opcion == 'E' || opcion == 'e') 
        {
            if (tablero->matriz[fila][columna].revelada) 
            {
                puts("Esta casilla ya fue revelada.");
                continue;
            }
            if (tablero->matriz[fila][columna].bandera) 
            {
                puts("No se puede revelar una casilla sospechosa");
                continue;
            }

            if (tablero->matriz[fila][columna].es_mina) 
            {
                // Revelar todas las minas al perder
                for (int i = 0; i < tablero->filas; i++) 
                {
                    for (int j = 0; j < tablero->columnas; j++) 
                    {
                        if (tablero->matriz[i][j].es_mina) tablero->matriz[i][j].revelada = true;
                    }
                }
                mostrar_tableroBM(tablero);
                puts("");
                puts("=================================");
                puts(" Tocaste una mina. GAME OVER.");
                puts("=================================");
                break;
            }

            if (tablero->matriz[fila][columna].minas_alrededor == 0) 
            {
                bfs(tablero, fila, columna);
            } 
            else 
            {
                tablero->matriz[fila][columna].revelada = true;
            }

            if (condicion_victoriaBM(tablero))
            {
                mostrar_tableroBM;
                puts("");
                puts("==============================");
                puts("    Felicidades Has Ganado");
                puts("==============================");

            }
        }
    }
}


void buscaminas()
{
    char opcion;
    TableroBM* tablero = NULL;

    do
    {
        mostrar_menuBM();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch(opcion)
        {
            case '1':
                tablero = crear_tablero_buscaminas(8, 8, 10);
                jugarBM(tablero);
                liberar_memoriaBM(tablero);
                break;

            case '2':
                tablero = crear_tablero_buscaminas(16, 16, 40);
                jugarBM(tablero);
                liberar_memoriaBM(tablero);
                break;

            case '3':
                tablero = crear_tablero_buscaminas(24, 24, 99);
                jugarBM(tablero);
                liberar_memoriaBM(tablero);
                break;

            case '4':
                puts("Volviendo al menu principal, gracias por jugar buscaminas.");
                break;

            default:
                puts("Esta opción no existe, ingrese una opción correcta.");
                break;
        }
    } while (opcion != '4');
}