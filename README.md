primera version de gato.
primera version conecta 4.

si ven algo raro haganmelo saber para explicarles igual documente un poco para que entienda un poco
porfa leanlo 
## Como correr el gato 
````
gcc -I. tdas/*.c juegos/*.c main.c -o programa
````
lo que hace tdas/*.c es correr todos los archivos de la carpeta de tdas, despues se compilan los demas archivos gato.c y main.c

Y luego ejecutar:
````
./programa.exe   
````
utilicen el .exe porque despues parece que va a servir para usarlo con tkinter para la interfaz

cosas a mejorar:
- preguntar por el nombre del jugador antes de empezar el juego
- ver como calcular los puntajes, y donde colocar la lista de puntajes (se podria hacer un lista como funcion en gato.h tipo List* puntajes(), etc)
- poner lo que dijo el henriquez del while true 
- parece que cual se ingresa una opcion no valida en el pve se salta el turno de la cpu
- falta volver al menu principal cuando se gana en el buscaminas
- falta poner una alerta para cuando la casilla ya este elegida en el buscaminas

