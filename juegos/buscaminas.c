#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "juegos/ranking.h"

// BM = buscaminas.

typedef struct
{
    bool es_mina;
    bool revelada;
    bool bandera;
    int minas_alrededor;
}CasillaBM;

typedef struct 
{
    int filas;
    int columnas;
    int num_minas;
    int puntaje;
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
                    printf("@  "); // @ para las minas. 
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

TableroBM* crear_tableroBM(int filas, int columnas, int num_minas)
{
    // se reserva memoria para el tablero
    TableroBM* tablero = (TableroBM*)malloc(sizeof(TableroBM));
    tablero->filas = filas;
    tablero->columnas = columnas;
    tablero->num_minas = num_minas;
    // por comodidad altere el struct agregando la variable puntaje.
    tablero->puntaje = 0;

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

// esta función reinicia el tablero a su estado inicial, indispensable para el bucle.
// PD: si van a quitar el bucle, quitar todo lo relacionado a este para evitar errores.
void reiniciar_tableroBM (TableroBM* tablero)
{
    for (int fila = 0;  fila < tablero->filas ; fila++)
    {
        for (int col = 0; col < tablero->columnas ; col++)
        {
            tablero->matriz[fila][col].es_mina = false;
            tablero->matriz[fila][col].revelada = false;
            tablero->matriz[fila][col].bandera = false;
            tablero->matriz[fila][col].minas_alrededor = 0;
        }
    }
}

void poner_minas(TableroBM* tablero, int fila, int columna)
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
            // esta condicion asegura que no explote una mina en el primer turno...
            if (filaAzar == fila && columnaAzar == columna) continue;
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
            // se traspasa el valor a la casilla correspondiente.
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
    // Se traspasa la coordenada inicial a la cola
    Coord* pos_i = (Coord*)malloc(sizeof(Coord));
    pos_i->x = pos_f;
    pos_i->y = pos_c;

    List* cola = list_create();
    list_pushBack(cola, pos_i);

    tablero->matriz[pos_f][pos_c].revelada = true;
    // se revela la casilla y se le otorgan 10 pts al puntaje final.
    tablero->puntaje += 10;

    while(list_size(cola) > 0)
    {
        // se extrae una posicion por visitar.
        Coord* actual = (Coord*)list_popFront(cola);; 

        if (tablero->matriz[actual->x][actual->y].minas_alrededor > 0) continue;

        // se agregan las casillas vecinas.
        for (int rec_f = -1; rec_f <= 1; rec_f++) 
        {
            for (int rec_c = -1; rec_c <= 1; rec_c++) 
            {
                int vecino_fil = actual->x + rec_f;
                int vecino_col = actual->y + rec_c;

                // se verifica que se encuentre dentro del tablero.
                if (vecino_fil >= 0 && vecino_fil < tablero->filas && vecino_col >= 0 && vecino_col < tablero->columnas) 
                {
                    // se revelan los vecinos que no son minas.
                    if (tablero->matriz[vecino_fil][vecino_col].revelada == false && tablero->matriz[vecino_fil][vecino_col].es_mina == false) 
                    {
                        tablero->matriz[vecino_fil][vecino_col].revelada = true;
                        tablero->puntaje += 10;

                        // se verifica si el vecino tiene 0 minas alrededor.
                        if (tablero->matriz[vecino_fil][vecino_col].minas_alrededor == 0) 
                        {
                            // se añade a la cola para expandirlo.
                            Coord* vecino = (Coord*)malloc(sizeof(Coord));
                            vecino->x = vecino_fil;
                            vecino->y = vecino_col;
                            list_pushBack(cola, vecino);
                        }
                    }
                }
            }
        }
        free(actual);
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

void jugarBM(TableroBM* tablero, char dificultad)
{
    int fila;
    int columna;
    char opcion;
    bool primer_mov = true;
    int puntaje_victoria;

    // dependiendo de la dificultad es el puntaje que se otorga cuando se gana.
    if (dificultad == '1') puntaje_victoria = 200;
    else if (dificultad == '2') puntaje_victoria = 500;
    else puntaje_victoria = 1000;

    while(true)
    {
        mostrar_tableroBM(tablero);
        puts("Movimientos: [E] Revelar Casilla o [F] Poner/Quitar Bandera");
        puts("Ejemplo -> 7 7 E / (FILA, COLUMNA, MOVIMIENTO).");

        scanf("%d %d %c", &fila, &columna, &opcion);

        // se le resta 1 a cada parametro porque si no se desfasa la posición.
        fila--;
        columna--;

        // se verifica que se encuentre dentro del tablero.
        if (fila < 0 || fila >= tablero->filas || columna < 0 || columna >= tablero->columnas) 
        {
            puts("");
            puts("Casilla no valida.");
            puts("");
            presioneTeclaParaContinuar();
            continue;
        }

        // opcion poner banderas.
        if (opcion == 'F' || opcion == 'f') 
        {
            if (tablero->matriz[fila][columna].revelada == true) 
            {
                puts("");
                puts("No puedes poner bandera en una casilla ya revelada.");
                puts("");
                presioneTeclaParaContinuar();
            }
            else 
            {
                if (tablero->matriz[fila][columna].bandera == true) tablero->matriz[fila][columna].bandera = false;
                else tablero->matriz[fila][columna].bandera = true;
            }
            continue;
        }

        // opcion revelar casillas.
        if (opcion == 'E' || opcion == 'e') 
        {

            // Se genera el tablero despues de hacer la primera jugada.
            // de esta forma nos aseguramos de que no explote en el primer turno.
            if (primer_mov == true)
            {
                poner_minas(tablero, fila, columna);
                valor_minas_adyacentes(tablero);
                primer_mov = false;
            }

            // Avisos
            if (tablero->matriz[fila][columna].revelada == true) 
            {
                puts("");
                puts("Esta casilla ya fue revelada.");
                puts("");
                presioneTeclaParaContinuar();
                continue;
            }

            if (tablero->matriz[fila][columna].bandera == true) 
            {
                puts("");
                puts("No se puede revelar una casilla sospechosa");
                puts("");
                presioneTeclaParaContinuar();
                continue;
            }

            if (tablero->matriz[fila][columna].es_mina == true) 
            {
                // Revelar todas las minas al perder
                for (int i = 0; i < tablero->filas; i++) 
                {
                    for (int j = 0; j < tablero->columnas; j++) 
                    {
                        if (tablero->matriz[i][j].es_mina == true) tablero->matriz[i][j].revelada = true;
                    }
                }
                mostrar_tableroBM(tablero);
                puts("");
                puts("=================================");
                puts(" Tocaste una mina. GAME OVER.");
                puts("=================================");
                puts("");
                // mostrar el puntaje al perder (provisional).
                printf("%d\n\n", tablero->puntaje);
                registrar_puntaje(JUEGA_BUSCAMINAS, tablero->puntaje);
                presioneTeclaParaContinuar();
                break;
            }

            // revelar casillas.
            if (tablero->matriz[fila][columna].minas_alrededor == 0) 
            {
                bfs(tablero, fila, columna);
            } 
            else 
            {
                tablero->matriz[fila][columna].revelada = true;
                tablero->puntaje += 10;
            }

            // se asegura que la cantidad de minas son iguales a la cantidad
            // de casillas por revelar, ganando asi el juego.
            if (condicion_victoriaBM(tablero))
            {
                mostrar_tableroBM(tablero);
                puts("");
                puts("==============================");
                printf("     Has Ganado +%d PTS\n", puntaje_victoria);
                puts("==============================");
                puts("");
                // se le otorga puntaje segun la dificultad.
                tablero->puntaje += puntaje_victoria;
                // se reinicia el tablero para mantener la continuidad del juego.
                // la condicion de primer movimiento, vuelve a ser verdadera.
                // por lo tanto al jugar el siguiente turno se regenerara el tablero.
                reiniciar_tableroBM(tablero);
                primer_mov = true;
                presioneTeclaParaContinuar();
                continue;
            }
        }
    }
}

void buscaminas()
{
    char opcion;
    int puntaje;
    TableroBM* tablero = NULL;

    do
    {
        mostrar_menuBM();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        // opciones del submenu del buscaminas.
        switch(opcion)
        {
            case '1':
                tablero = crear_tableroBM(8, 8, 10);
                jugarBM(tablero, '1');
                liberar_memoriaBM(tablero);
                break;

            case '2':
                tablero = crear_tableroBM(16, 16, 40);
                jugarBM(tablero, '2');
                liberar_memoriaBM(tablero);
                break;

            case '3':
                tablero = crear_tableroBM(24, 24, 99);
                jugarBM(tablero, '3');
                liberar_memoriaBM(tablero);
                break;

            case '4':
                puts("");
                puts("Volviendo al menu principal, gracias por jugar buscaminas.");
                puts("");
                presioneTeclaParaContinuar();
                break;

                // caso de prueba oculto al usuario.
                // nos facilita verificar que pasa en casos de victoria
                // ya que es un tablero 3 x 3.
            case '9':
                tablero = crear_tableroBM(3, 3, 2);
                jugarBM(tablero, '3');
                liberar_memoriaBM(tablero);
                break;

            default:
                puts("");
                puts("Esta opción no existe, ingrese una opción correcta.");
                puts("");
                presioneTeclaParaContinuar();
                break;
        }
    } while (opcion != '4');
}
