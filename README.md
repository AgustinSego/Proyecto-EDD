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
- preguntar por el nombre del jugador antes de empezar el juego *Propuesta*
Propongo preguntar el nombre al finalizar de jugar en modos PvE, de esta forma nos resultara más facil rellenar la lista de puntajes. No se si nos convendra hacer un sistema de almacenamiento de puntajes, algo que exporte un excel y despues al iniciar el programa se pueda leer y guardar de esa forma los rankings respectivos...

- preguntar por el nombre del jugador antes de empezar el juego *Propuesta*
Propongo preguntar el nombre al finalizar de jugar en modos PvE, de esta forma nos resultara más facil rellenar la lista de puntajes. No se si nos convendra hacer un sistema de almacenamiento de puntajes, algo que exporte un excel y despues al iniciar el programa se pueda leer y guardar de esa forma los rankings respectivos...

- ver como calcular los puntajes, y donde colocar la lista de puntajes (se podria hacer un lista como funcion en gato.h tipo List* puntajes(), etc)

- Agregar el MODO ARCADE, es importante agregar una funcion de este tipo ya que va de la mano con el puntaje, de esta forma el puntaje se acumula y no lo pierdes si ganas. Asi al momento de perder se le solicita el nombre al jugador y se insertan el nombre y su puntaje en el arbol binario correspondiente al ranking del juego.
*Agregue el bucle arcade dentro del buscaminas + un sistema de puntaje provisorio (creo que se puede mejorar)*


- Agregar el MODO ARCADE, es importante agregar una funcion de este tipo ya que va de la mano con el puntaje, de esta forma el puntaje se acumula y no lo pierdes si ganas. Asi al momento de perder se le solicita el nombre al jugador y se insertan el nombre y su puntaje en el arbol binario correspondiente al ranking del juego.
*Agregue el bucle arcade dentro del buscaminas + un sistema de puntaje provisorio (creo que se puede mejorar)*

- parece que cual se ingresa una opcion no valida en el pve se salta el turno de la cpu

- ~~falta volver al menu principal cuando se gana en el buscaminas~~ Solucionado...

- ~~falta poner una alerta para cuando la casilla ya este elegida en el buscaminas~~ 
*Arregle los avisos cuando se cometen errores, ahora se obliga al usuario de presionar una tecla obligandolo a leer antes de continuar (esto en el buscaminas).*

- ~~Cuando se accede al conecta 4, y se intenta volver al menu principal, el programa tiende a cerrarse.~~ Arreglado...

- ~~Explota una mina en el primer turno del buscaminas~~ Arreglado...

## Comentarios:

- para probar los casos en los que se gana en el buscaminas, se puede utilizar el numero nueve en el submenu del buscaminas, los llevara a jugar con un tablero de 3x3 más facil de ganar.

- Documente el juego del buscaminas, asi les sera más facil saber que hace cada función en caso de que vayan a editarlo (Si no entienden algo avisenme y les explicare).

- ~~falta volver al menu principal cuando se gana en el buscaminas~~ Solucionado...

- ~~falta poner una alerta para cuando la casilla ya este elegida en el buscaminas~~ 
*Arregle los avisos cuando se cometen errores, ahora se obliga al usuario de presionar una tecla obligandolo a leer antes de continuar (esto en el buscaminas).*

- ~~Cuando se accede al conecta 4, y se intenta volver al menu principal, el programa tiende a cerrarse.~~ Arreglado...

- ~~Explota una mina en el primer turno del buscaminas~~ Arreglado...

## Comentarios:

- para probar los casos en los que se gana en el buscaminas, se puede utilizar el numero nueve en el submenu del buscaminas, los llevara a jugar con un tablero de 3x3 más facil de ganar.

- Documente el juego del buscaminas, asi les sera más facil saber que hace cada función en caso de que vayan a editarlo (Si no entienden algo avisenme y les explicare).
