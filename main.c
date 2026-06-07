#include <stdio.h>
#include "tdas/extra.h"
#include "juegos/gato.h"

void mostrar_menu(){
    limpiarPantalla();
    puts("=====================================================");
    puts("     Game-Hub");
    puts("=====================================================");

    puts("1) Jugar al Gato");
    puts("2) Salir De Game-Hub");
    
}

int main(){
    char opcion;

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
            puts("Saliendo de Game-Hub");
            puts("Hasta la proxima");
            break;
        default:
        puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();
    } while(opcion != '2');

    return 0;
}