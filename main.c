#include <stdio.h>
#include "tdas/extra.h"
#include "juegos/gato.h"
#include "juegos/conecta4.h"
#include "juegos/buscaminas.h"
#include "ranking.h"


void mostrar_menu(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub");
    puts("=====================================================");

    puts("1) Jugar al Gato");
    puts("2) Jugar Conecta 4");
    puts("3) Jugar Buscaminas");
    puts("4) Ver Rankings del Hub");
    puts("5) Salir de Game-Hub");
}

int main(){
    char opcion;

    incializar_rankings();
    
    do
    {
        mostrar_menu();
        printf("Bienvenido a Game-Hub ingrese una opcion: ");
        scanf(" %c", &opcion);

        switch(opcion){
        case '1':
            gato();
            break;
        case '2':
            conecta4();
            break;
        case '3':
            buscaminas();
            break;
        case '4':
            mostrar_ranking();
            break;
        case '5':
            puts("Saliendo de Game-Hub");
            puts("Hasta la proxima");
            break;
        default:
        puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();
    } while(opcion != '5');
    liberar_ranking();
    return 0;
}