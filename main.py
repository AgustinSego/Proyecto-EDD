import ctypes
import tkinter as tk
from tkinter import messagebox
import random

# =====================================================================
# 1. CARGAR LIBRERÍA DE C Y CONFIGURAR MEMORIA
# =====================================================================
# Asegúrate de que el nombre coincida con tu archivo .dll
lib = ctypes.CDLL('./libgato.dll') 

TableroC = (ctypes.c_char * 3) * 3
lib.ganador.argtypes = [ctypes.c_char, TableroC]
lib.ganador.restype = ctypes.c_int
lib.empate.argtypes = [TableroC]
lib.empate.restype = ctypes.c_int
lib.movimiento_aleatorio.argtypes = [TableroC, ctypes.c_char]
lib.movimiento_aleatorio.restype = None

# Inicializar el tablero en la memoria compartida
tablero_c = TableroC(
    (b'1', b'2', b'3'),
    (b'4', b'5', b'6'),
    (b'7', b'8', b'9')
)

# Variables globales para el estado del juego
modo_juego = "pvp"
dificultad_actual = "1"
jugador_actual = b'X'

# =====================================================================
# 2. LÓGICA DE INTERFAZ Y REGLAS
# =====================================================================
def cambiar_pantalla(frame_destino):
    frame_principal.place_forget()
    frame_menu_gato.place_forget()
    frame_dificultad_pve.place_forget()
    frame_juego.place_forget()
    
    # El menú destino se expande al 100% de la pantallita de 175x100
    frame_destino.place(x=0, y=0, relwidth=1, relheight=1)

def iniciar_modo(modo, dificultad="1"):
    global modo_juego, jugador_actual, dificultad_actual
    modo_juego = modo
    dificultad_actual = dificultad
    jugador_actual = b'X'
    reiniciar_juego()
    cambiar_pantalla(frame_juego)

def reiniciar_juego():
    global jugador_actual
    jugador_actual = b'X'
    valores = [b'1', b'2', b'3', b'4', b'5', b'6', b'7', b'8', b'9']
    idx = 0
    for i in range(3):
        for j in range(3):
            tablero_c[i][j] = valores[idx]
            botones[i][j].config(text="", state="normal")
            idx += 1

def actualizar_interfaz_desde_memoria():
    for i in range(3):
        for j in range(3):
            if tablero_c[i][j] == b'O' and botones[i][j].cget("text") == "":
                botones[i][j].config(text="O", fg="red")

def click_boton(fila, columna):
    global jugador_actual
    if tablero_c[fila][columna] in (b'X', b'O'): return

    if modo_juego == "pvp":
        tablero_c[fila][columna] = jugador_actual
        botones[fila][columna].config(text=jugador_actual.decode(), fg="blue" if jugador_actual == b'X' else "red")
        
        if lib.ganador(jugador_actual, tablero_c) == 1:
            messagebox.showinfo("Game Hub", f"¡Jugador {jugador_actual.decode()} gana!")
            cambiar_pantalla(frame_menu_gato)
            return
        if lib.empate(tablero_c) == 1:
            messagebox.showinfo("Game Hub", "¡Es un empate!")
            cambiar_pantalla(frame_menu_gato)
            return
        jugador_actual = b'O' if jugador_actual == b'X' else b'X'
    else:
        # Turno Jugador PvE
        tablero_c[fila][columna] = b'X'
        botones[fila][columna].config(text="X", fg="blue")

        if lib.ganador(b'X', tablero_c) == 1:
            messagebox.showinfo("Game Hub", "¡Felicidades, ganaste!")
            cambiar_pantalla(frame_menu_gato)
            return
        if lib.empate(tablero_c) == 1:
            messagebox.showinfo("Game Hub", "¡Es un empate!")
            cambiar_pantalla(frame_menu_gato)
            return

        # Turno CPU PvE
        dado = random.randint(1, 100)
        if dificultad_actual == "1": lib.movimiento_aleatorio(tablero_c, b'O')
        elif dificultad_actual == "2": lib.movimiento_aleatorio(tablero_c, b'O')
        elif dificultad_actual == "3": lib.movimiento_aleatorio(tablero_c, b'O')
        actualizar_interfaz_desde_memoria()

        if lib.ganador(b'O', tablero_c) == 1:
            messagebox.showinfo("Game Hub", "La CPU gana esta vez.")
            cambiar_pantalla(frame_menu_gato)
            return
        if lib.empate(tablero_c) == 1:
            messagebox.showinfo("Game Hub", "¡Es un empate!")
            cambiar_pantalla(frame_menu_gato)
            return

# =====================================================================
# 3. CONFIGURACIÓN VISUAL MAESTRA
# =====================================================================
ventana = tk.Tk()
ventana.title("Game-Hub Arcade")
ventana.geometry("1080x1280") #1920x1080

# Cargar la imagen del arcade
try:
    img_arcade = tk.PhotoImage(file="banner .png")

    img_arcade = img_arcade.zoom(2)

    fondo = tk.Label(ventana, image=img_arcade)
    fondo.place(relx=0.5, rely=0.5, anchor="center")
except tk.TclError:
    pass

# Cargar la textura de pantalla rayada
try:
    img_pantalla = tk.PhotoImage(file="pantalla.png")
except tk.TclError:
    img_pantalla = None

BG_TEXTO = "#8ab1d4" 

# --- MARCO DE LA PANTALLA (¡Esta es la línea que te falta!) ---
pantalla_arcade = tk.Frame(ventana)

# =====================================================================
# ZONA DE CALIBRACIÓN: El ajuste milimétrico
# =====================================================================
ANCHO = 290       # Lo bajamos para que deje de pisar los bordes rojos laterales
ALTO = 210        # Lo bajamos para que no se coma la parte de arriba y de abajo

EJE_X_REL = 0.5   # Mantenemos el centro horizontal perfecto
EJE_Y_REL = 0.33  # Lo bajamos una pizca minúscula para que no pise arriba

pantalla_arcade.place(relx=EJE_X_REL, rely=EJE_Y_REL, width=ANCHO, height=ALTO, anchor="center")
# =====================================================================

# =====================================================================
# 4. CONSTRUCCIÓN DE MENÚS MINIFICADOS
# =====================================================================
def aplicar_papel_tapiz(frame):
    if img_pantalla:
        fondo_rayado = tk.Label(frame, image=img_pantalla)
        fondo_rayado.place(x=0, y=0, relwidth=1, relheight=1)

# Fuente muy pequeña para que quepa en la pantalla virtual
FUENTE = ("Courier", 15, "bold") 

# --- Menú Principal ---
frame_principal = tk.Frame(pantalla_arcade)
aplicar_papel_tapiz(frame_principal)

tk.Label(frame_principal, text="- GAME HUB -", font=("Courier", 10, "bold"), bg=BG_TEXTO).pack(pady=2)
tk.Button(frame_principal, text="1) Jugar al Gato", font=FUENTE, width=15, command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=1)
tk.Button(frame_principal, text="2) Salir", font=FUENTE, width=15, fg="red", command=ventana.quit).pack(pady=1)

# --- Sub Menú Gato ---
frame_menu_gato = tk.Frame(pantalla_arcade)
aplicar_papel_tapiz(frame_menu_gato)

tk.Label(frame_menu_gato, text="- MODO -", font=("Courier", 10, "bold"), bg=BG_TEXTO).pack(pady=2)
tk.Button(frame_menu_gato, text="1) PvP", font=FUENTE, width=15, command=lambda: iniciar_modo("pvp")).pack(pady=1)
tk.Button(frame_menu_gato, text="2) PvE", font=FUENTE, width=15, command=lambda: cambiar_pantalla(frame_dificultad_pve)).pack(pady=1)
tk.Button(frame_menu_gato, text="Volver", font=FUENTE, width=15, command=lambda: cambiar_pantalla(frame_principal)).pack(pady=1)

# --- Dificultad PvE ---
frame_dificultad_pve = tk.Frame(pantalla_arcade)
aplicar_papel_tapiz(frame_dificultad_pve)

tk.Label(frame_dificultad_pve, text="- DIFICULTAD -", font=("Courier", 8, "bold"), bg=BG_TEXTO).pack(pady=1)
tk.Button(frame_dificultad_pve, text="1) Facil", font=FUENTE, width=15, command=lambda: iniciar_modo("pve", "1")).pack(pady=1)
tk.Button(frame_dificultad_pve, text="2) Normal", font=FUENTE, width=15, command=lambda: iniciar_modo("pve", "2")).pack(pady=1)
tk.Button(frame_dificultad_pve, text="3) Dificil", font=FUENTE, width=15, command=lambda: iniciar_modo("pve", "3")).pack(pady=1)
tk.Button(frame_dificultad_pve, text="Volver", font=FUENTE, width=15, command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=1)

# --- Juego (Tablero miniatura) ---
frame_juego = tk.Frame(pantalla_arcade)
aplicar_papel_tapiz(frame_juego)

cuadricula = tk.Frame(frame_juego, bg=BG_TEXTO)
cuadricula.pack(pady=2)

botones = [[None for _ in range(3)] for _ in range(3)]
for i in range(3):
    for j in range(3):
        # Botones pequeños para encajar en la cuadricula ajustada
        btn = tk.Button(cuadricula, text="", font=('Arial', 8, 'bold'), width=2, height=1,
                        command=lambda f=i, c=j: click_boton(f, c))
        btn.grid(row=i, column=j, padx=1, pady=1)
        botones[i][j] = btn

tk.Button(frame_juego, text="Abandonar", font=("Courier", 7, "bold"), bg="lightgray", command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=2)

# =====================================================================
# 5. INICIO DE LA APLICACIÓN
# =====================================================================
cambiar_pantalla(frame_principal)
ventana.mainloop()