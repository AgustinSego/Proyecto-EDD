# GAME-HUB

## Descripción del programa:
Este programa es una hub interactiva que funciona de forma local que reúne tres videojuegos clásicos: Gato, Conecta 4 y Buscaminas. Este sistema permite jugar partidas con tus amigos de forma local con el modo PvP (Jugador contra Jugador) y ponerte a prueba con la modalidad PvE (Jugador contra Máquina) contando con tres niveles de dificultad (Fácil, Normal y Difícil). Para los modos PvE se implementa una inteligencia artificial para la toma de decisiones de la máquina. También se cuenta con un sistema de bucles, un sistema de puntuaciones y rankings de cada juego recreando la esencia de los juegos de arcade.

## Información y Requisitos Para Compilar:
- Tener VSCode instalado.
- Tener instalada la extensión de Microsoft C/C++. 
- Tener instalado un compilador de C como GCC.
- Tener instalado Python.

## Cómo compilar (Visual Studio Code):
1. Descargar el repositorio de GITHUB como zip.
2. Descomprime el archivo zip de GAME-HUB y guarda la carpeta.
3. Abre VSCode y abre la carpeta, pulsando "File"->"Open Folder".
4. Abre la terminal integrada y ejecuta los siguientes comandos:

GAME-HUB con interfaz gráfica:
````
py main.py
````

GAME-HUB sin interfaz gráfica (interfaz consola):
````
./programa.exe
````

## Funcionalidades

### Funcionando correctamente:
- Jugar Gato y sus funciones derivadas.
- Jugar Conecta 4 y sus funciones derivadas.
- Jugar Buscaminas y sus funciones derivadas.
- Ranking y sus funciones derivadas.
- Interfaz Gráfica.

### A mejorar:
- Interfaz de la consola del programa, sentimos que se podría mejorar en varios aspectos.

### Problemas conocidos:
- No encontramos ningún problema que genere al momento de ejecutar el programa.

## Ejemplo de uso (Descriptivo):

### Paso 1: Seleccionar Juego.

![MenuPrincipal](/muestra/principal.png)

- Se nos muestra en esta imagen el menú principal de GAME-HUB donde podemos ver las distintas opciones, tanto los juegos, como el "Ver Ranking". Estas funcionan de forma interactiva con el cursor.

### Paso 2: Seleccionar Modalidad (**En este caso el GATO**).

![SubMenuGato](/muestra/submenu_gt.png)

- Podemos ver en esta imagen el submenú del Gato, donde podemos apreciar las opciones "Jugar PvP" y "Jugar PvE".

### Paso 3: Seleccionar Dificultad.

![DificultadesGato](/muestra/df_gt.png)

- Podemos apreciar en esta imagen como se nos muestran las distintas dificultades del Gato. Al seleccionar una empieza el juego.

### Paso 4: Jugar (**En este caso el GATO**).

![ExpModoArcadeGato](/muestra/exp_arcade.png)

- Podemos ver en la imagen como al ganar, se le otorga al jugador una bonificación de puntaje y continua el juego.

### Paso 5: Ingresar Apodo.

![Apodo](/muestra/apodo.png)

- Apreciamos en la imagen como al momento de perder la partida, se le pregunta al jugador su apodo, de esta forma se almacena el puntaje. Al perder termina el bucle y se vuelve al submenú del gato (esto replica de forma exacta el modo arcade que se quería conseguir). 

### Paso 6: Mostrar Ranking.

![Ranking](/muestra/rank.png)

- Se muestra la tabla de clasificación de los distintos juegos, donde se muestran únicamente los 3 mejores puntajes.

## Contribuciones:

### Agustín Henríquez (Coordinador):
- Implementación del juego Buscaminas y las funciones derivadas de esta.
- Implementación de los bucles de juego (modo arcade).
- Redacción del README.

### Agustín Segovia (Gestor de calidad):
- Implementación del juego del Gato en sus modalidades PvP y PvE en modo fácil.
- Implementación de la interfaz gráfica.

### Borja Urrea (Responsable de Integración y consistencia):
- Implementación del minimax del juego del Gato para los modos normal y difícil.
- Implementación del sistema de ranking y los sistemas de puntajes.

### Diego Andreoli (Comunicador):
- Implementación del juego Conecta 4 PvP y PvE (minimax).
- Implementación de funciones derivadas de Conecta 4.
