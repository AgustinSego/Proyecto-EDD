#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "conecta4.h"
#include "ranking.h"

#define P1 'A'
#define P2 'B'
#define filas 6
#define columnas 7

void destruir_grafo(nodo *nodo){
    if(nodo == NULL){
        return;
    }
    for(int i = 0; i < nodo->cantidad_hijos; i++){
        destruir_grafo(nodo->hijos[i]);
    }
    free(nodo);
}
int columna_valida(juego *j, int columna){
    return columna >= 0 && columna < columnas && j->table[0][columna] == '.';
}
void copiar_tablero(juego *origen, juego *destino){
    for(short i = 0; i < filas; i++){
        for(short k = 0; k < columnas; k++){
            destino->table[i][k] = origen->table[i][k];
        }
    }
}
nodo *crear_nodo(juego *j, int columna){

    nodo *nuevo = malloc(sizeof(nodo));
    if(nuevo == NULL) return NULL;

    nuevo->estado = *j;
    nuevo->columna_jugada = columna;
    nuevo->puntaje = 0;
    nuevo->cantidad_hijos = 0;

    for(int i = 0; i < max_hijos; i++){
        nuevo->hijos[i] = NULL;
    }

    return nuevo;
}
void movimiento_nodo(nodo *n, char jugador){
    n->cantidad_hijos = 0;

    for(int col = 0; col < columnas; col++){

        if(!columna_valida(&n->estado, col)){
            continue;
        }

        juego copia = n->estado;

        if(!jugada(&copia, col, jugador)){
            continue;
        }

        nodo *hijo = crear_nodo(&copia, col);
        if(hijo == NULL) continue;

        n->hijos[n->cantidad_hijos++] = hijo;
    }
}
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
int evaluar_ventana(char ventana[4], char simbolo){

    char rival;

    if(simbolo == P2){
        rival = P1;
    }
    else{
        rival = P2;
    }

    int propias = 0;
    int enemigas = 0;
    int vacias = 0;

    for(int i = 0; i < 4; i++){
        if(ventana[i] == simbolo)
            propias++;
        else if(ventana[i] == rival)
            enemigas++;
        else
            vacias++;
    }

    int puntaje = 0;

    if(propias == 4)
        puntaje += 100000;

    else if(propias == 3 && vacias == 1)
        puntaje += 100;

    else if(propias == 2 && vacias == 2)
        puntaje += 10;

    if(enemigas == 3 && vacias == 1)
        puntaje -= 120;

    return puntaje;
}
int evaluar_tablero(juego *j, char simbolo){
    int puntaje = 0;
    char ventana[4];
    for(int i = 0; i < filas; i++){
        if(j->table[i][3] == simbolo)
            puntaje += 6;
    }
    for(int i = 0; i < filas; i++){
        for(int k = 0; k < columnas - 3; k++){
            for(int n = 0; n < 4; n++)
                ventana[n] = j->table[i][k + n];

            puntaje += evaluar_ventana(ventana, simbolo);
        }
    }
    for(int i = 0; i < filas - 3; i++){
        for(int k = 0; k < columnas; k++){
            for(int n = 0; n < 4; n++)
                ventana[n] = j->table[i + n][k];

            puntaje += evaluar_ventana(ventana, simbolo);
        }
    }
    for(int i = 0; i < filas - 3; i++){
        for(int k = 0; k < columnas - 3; k++){
            for(int n = 0; n < 4; n++)
                ventana[n] = j->table[i + n][k + n];

            puntaje += evaluar_ventana(ventana, simbolo);
        }
    }
    for(int i = 0; i < filas - 3; i++){
        for(int k = 3; k < columnas; k++){
            for(int n = 0; n < 4; n++)
                ventana[n] = j->table[i + n][k - n];

            puntaje += evaluar_ventana(ventana, simbolo);
        }
    }

    return puntaje;

}
int minimax_conecta4(nodo *n, int profundidad, int alfa, int beta, int es_max, char jugador_actual){

    if(ganaron_c4(&n->estado, P2))
        return 1000000 + profundidad;


    if(ganaron_c4(&n->estado, P1))
        return -1000000 - profundidad;


    if(empate_c4(&n->estado) || profundidad == 0){
        return evaluar_tablero(&n->estado, P2);
    }

    char siguiente = (jugador_actual == P2) ? P1 : P2;

    if(n->cantidad_hijos == 0){
        movimiento_nodo(n, siguiente);
    }

    if(es_max){
        int mejor = -INF;

        for(int i = 0; i < n->cantidad_hijos; i++){

            int valor = minimax_conecta4(n->hijos[i], profundidad - 1, alfa, beta, 0, siguiente);

            if(valor > mejor)
                mejor = valor;

            if(valor > alfa)
                alfa = valor;

            if(beta <= alfa)
                break;
        }

        n->puntaje = mejor;
        return mejor;
    }
    else{
        int mejor = INF;

        for(int i = 0; i < n->cantidad_hijos; i++){

            int valor = minimax_conecta4(n->hijos[i],profundidad - 1,alfa,beta,1,siguiente);

            if(valor < mejor)
                mejor = valor;

            if(valor < beta)
                beta = valor;

            if(beta <= alfa)
                break;
        }

        n->puntaje = mejor;
        return mejor;
    }
}
int maquina_facil(juego *j){
    int columna;

    do{
        columna = rand() %columnas;
    }while(!columna_valida(j,columna));

    return columna;

}
int maquina_media(juego *j){
    juego copia;

    for(int col = 0; col < columnas; col++){
        if(!columna_valida(j,col)){
            continue;
        }
        copiar_tablero(j,&copia);
        jugada(&copia,col,P2);
        if(ganaron_c4(&copia, P2)){
            return col;
        }
    }
    for(int col = 0; col < columnas; col++){
        if(!columna_valida(j,col)){
            continue;
        }
        copiar_tablero(j,&copia);
        jugada(&copia,col,P1);
        if(ganaron_c4(&copia, P1)){
            return col;
        }
    }

    if(columna_valida(j,3)){
        return 3;        
    }
    return maquina_facil(j);
}
int maquina_dificil(juego *j){

    nodo *raiz = crear_nodo(j, -1);
    movimiento_nodo(raiz, P2);

    if(raiz->cantidad_hijos == 0){
        destruir_grafo(raiz);
        return maquina_facil(j);
    }

    int mejor_columna = -1;
    int mejor_puntaje = -INF;

    for(int i = 0; i < raiz->cantidad_hijos; i++){

        nodo *hijo = raiz->hijos[i];
        if(hijo == NULL) continue;

        int puntaje = minimax_conecta4(hijo, 5, -INF, INF, 0,P2);

        if(puntaje > mejor_puntaje){
            mejor_puntaje = puntaje;
            mejor_columna = hijo->columna_jugada;
        }
    }

    destruir_grafo(raiz);

    if(mejor_columna < 0 || mejor_columna >= columnas)
        return maquina_facil(j);

    return mejor_columna;
}
void pvp_c4(juego *j){
    int posicion;
    char simbolo = P1;
    int fichas_jugadas = 0;

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

        fichas_jugadas ++;

        if(empate_c4(j)){
            printf("EMPATARON\n");
            break;
        }
        if(ganaron_c4(j,simbolo)){
            pintar(j);
            printf("El jugador %c gano\n", simbolo);
            //100 puntos base mas 15 puntos por cada espacio vacio que sobro en el tablero: a menos movimientos para ganar = mas espacios vacios
            int puntaje_calculado = 100 + ((42-fichas_jugadas) * 15);

            printf("¡Ganaste usando solo %d fichas en total!\nTu puntuacion es %d puntos", fichas_jugadas, puntaje_calculado);
            registrar_puntaje(JUEGA_CONECTA4, puntaje_calculado);
            break;
        }

        if(simbolo == P1) simbolo = P2;
        else simbolo = P1;
    }
}
// MODIFICACION IMPLEMENTACION DE BUCLE (MODO ARCADE).
void pve_c4(juego *j, int dificultad){
    List *historia = list_create();
    int puntaje = 0;
    int puntaje_victoriaC4;
    int puntaje_empateC4;

    if (dificultad == 1) puntaje_victoriaC4 = 250;
    else if (dificultad == 2) puntaje_victoriaC4 = 500;
    else puntaje_victoriaC4 = 1000;

    if (dificultad == 1) puntaje_empateC4 = 100;
    else if (dificultad == 2) puntaje_empateC4 = 200;
    else puntaje_empateC4 = 400;

    while(1){
        int posicion;
        pintar(j);

        printf("jugador %c, ingrese una columna: ", P1);
        scanf("%d", &posicion);
        if (posicion < 0 || posicion > columnas){
            printf("columna invalida\n");
            continue;
        }

        int columna = posicion -1;
        if(!jugada(j,columna,P1))
            continue;

        movimiento *mov = malloc(sizeof(movimiento));

        mov->jugador = P1;
        mov->columna = columna;
        list_pushBack(historia,mov);

        // SI SE GANA O SE EMPATA, SE REINICIA EL TABLERO.
        if(ganaron_c4(j,P1)){
            pintar(j);
            printf("!!!!ganaste\n");
            puntaje += puntaje_victoriaC4;
            puts("");
            printf("%d\n", puntaje);
            reiniciar_tablero_c4(j);
            //registrar_puntaje(JUEGA_CONECTA4, 100);
            continue;
        }
        if(empate_c4(j)){
            pintar(j);
            printf("empataron");
            puntaje += puntaje_empateC4;
            puts("");
            printf("%d\n", puntaje);
            reiniciar_tablero_c4(j);
            continue;
        }
        int mov_maquina;

        switch(dificultad){
            case 1:
                mov_maquina = maquina_facil(j);
                break;
            case 2:
                mov_maquina = maquina_media(j);
                break;
            case 3:
                mov_maquina = maquina_dificil(j);
                break;
            default:
                mov_maquina = maquina_facil(j);
        };
        printf("la maquina juega en la columna %d\n", mov_maquina +1);

        jugada(j,mov_maquina,P2);
        mov = malloc(sizeof(movimiento));

        mov->jugador = P2;
        mov->columna = mov_maquina;
        list_pushBack(historia,mov);
        if(ganaron_c4(j,P2)){
            pintar(j);
            printf("!!!!gano la maquina\n");
            registrar_puntaje(JUEGA_CONECTA4, puntaje);
            break;
        }
        if(empate_c4(j)){
            pintar(j);
            printf("empataron");
            puntaje += puntaje_empateC4;
            puts("");
            printf("%d\n", puntaje);
            reiniciar_tablero_c4(j);
            continue;
        }
    }
}
void pve_menu_c4(juego *j){
    char opcion;
    do{
            limpiarPantalla();
            puts("=====================================================");
            puts("     Game-Hub: menu pve");
            puts("=====================================================");

            puts("1) Jugar facil");
            puts("2) Jugar medio");
            puts("3) jugar dificil");
            puts("4) Volver al Menu Principal");
            printf("Ingrese una opcion de juego: ");
            scanf(" %c", &opcion);

            switch(opcion){
            case '1':
                reiniciar_tablero_c4(j);
                pve_c4(j,1);


                break;
            case '2':
                reiniciar_tablero_c4(j);
                pve_c4(j,2);

                break;
            case '3':
                reiniciar_tablero_c4(j);
                pve_c4(j,3);
                break;

            case '4':
                puts("Saliendo de conecta4");
                puts("Volviendo al menu principal");
                break;
            default:
                puts("Opción no válida. Por favor, intente de nuevo.");
            }
            presioneTeclaParaContinuar();
        }while(opcion != '4');


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
            pve_menu_c4(tablero);
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