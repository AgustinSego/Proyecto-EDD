import ctypes
import tkinter as tk
from tkinter import messagebox
import random

# --- 1. CARGAR LIBRERÍA Y CONFIGURAR MEMORIA (Sin cambios) ---
lib = ctypes.CDLL('./libgato.dll')
TableroC = (ctypes.c_char * 3) * 3
lib.ganador.argtypes = [ctypes.c_char, TableroC]
lib.ganador.restype = ctypes.c_int
lib.empate.argtypes = [TableroC]
lib.empate.restype = ctypes.c_int
lib.movimiento_aleatorio.argtypes = [TableroC, ctypes.c_char]
lib.movimiento_aleatorio.restype = None

tablero_c = TableroC(
    (b'1', b'2', b'3'),
    (b'4', b'5', b'6'),
    (b'7', b'8', b'9')
)

modo_juego = "pvp"
dificultad_actual = "1"
jugador_actual = b'X'

# --- 2. LÓGICA DE INTERFAZ (Sin cambios) ---
def cambiar_pantalla(frame_destino):
    frame_principal.place_forget()
    frame_menu_gato.place_forget()
    frame_dificultad_pve.place_forget()
    frame_juego.place_forget()
    
    # En lugar de pack, usamos place para centrarlos dentro de la pantallita
    frame_destino.place(relx=0.5, rely=0.5, anchor="center")

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


# --- 3. CONFIGURACIÓN VISUAL MAESTRA ---
ventana = tk.Tk()
ventana.title("Game-Hub Arcade")

# 1. Ajusta esto al tamaño exacto de la imagen que exportaste de Pixel Studio
ventana.geometry("1920x1080") 
ventana.resizable(False, False)

# 2. Poner la imagen del arcade como fondo global
try:
    img_arcade = tk.PhotoImage(file="banner .png") # Respetando el nombre de tu archivo
    img_arcade = img_arcade.zoom(2)
    fondo = tk.Label(ventana, image=img_arcade)
    fondo.place(x=0, y=0, relwidth=1, relheight=1)
except tk.TclError:
    messagebox.showerror("Error", "No se encontró el archivo 'banner .png'")

# 3. EXTRAER COLOR DE LA PANTALLA
# Este código hexadecimal es un azul muy parecido al de tu dibujo.
# Usaremos este color de fondo en los botones para que se camuflen en la pantalla.
BG_COLOR = "#b7d6e8" 

# 4. CREAR EL MARCO DE LA PANTALLA (AQUÍ ESTÁ LA MAGIA)
pantalla_arcade = tk.Frame(ventana, bg=BG_COLOR)

# =====================================================================
# ZONA DE CALIBRACIÓN: Juega con estos 4 números hasta que el cuadro 
# encaje perfectamente dentro del marco rojo de tu dibujo.
# =====================================================================
EJE_X = 800       # Mueve la pantalla a la izquierda/derecha
EJE_Y = 254       # Mueve la pantalla arriba/abajo
ANCHO = 320       # Qué tan ancha es la zona azul
ALTO = 220        # Qué tan alta es la zona azul

pantalla_arcade.place(x=EJE_X, y=EJE_Y, width=ANCHO, height=ALTO)
# =====================================================================


# --- 4. CREADOR DE MENÚS (Ahora todos viven dentro de 'pantalla_arcade') ---
FUENTE = ("Courier", 10, "bold") # Letra pequeña y retro para que quepa

# Menú Principal
frame_principal = tk.Frame(pantalla_arcade, bg=BG_COLOR)
tk.Label(frame_principal, text="- GAME HUB -", font=("Courier", 16, "bold"), bg=BG_COLOR).pack(pady=10)
tk.Button(frame_principal, text="1) Jugar al Gato", font=FUENTE, width=20, command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=5)
tk.Button(frame_principal, text="2) Salir", font=FUENTE, width=20, fg="red", command=ventana.quit).pack(pady=5)

# Sub Menú Gato
frame_menu_gato = tk.Frame(pantalla_arcade, bg=BG_COLOR)
tk.Label(frame_menu_gato, text="- MODO -", font=("Courier", 14, "bold"), bg=BG_COLOR).pack(pady=10)
tk.Button(frame_menu_gato, text="1) PvP", font=FUENTE, width=20, command=lambda: iniciar_modo("pvp")).pack(pady=5)
tk.Button(frame_menu_gato, text="2) PvE", font=FUENTE, width=20, command=lambda: cambiar_pantalla(frame_dificultad_pve)).pack(pady=5)
tk.Button(frame_menu_gato, text="Volver", font=FUENTE, width=20, command=lambda: cambiar_pantalla(frame_principal)).pack(pady=5)

# Dificultad PvE
frame_dificultad_pve = tk.Frame(pantalla_arcade, bg=BG_COLOR)
tk.Label(frame_dificultad_pve, text="- DIFICULTAD -", font=("Courier", 14, "bold"), bg=BG_COLOR).pack(pady=5)
tk.Button(frame_dificultad_pve, text="1) Facil", font=FUENTE, width=20, command=lambda: iniciar_modo("pve", "1")).pack(pady=2)
tk.Button(frame_dificultad_pve, text="2) Normal", font=FUENTE, width=20, command=lambda: iniciar_modo("pve", "2")).pack(pady=2)
tk.Button(frame_dificultad_pve, text="3) Dificil", font=FUENTE, width=20, command=lambda: iniciar_modo("pve", "3")).pack(pady=2)
tk.Button(frame_dificultad_pve, text="Volver", font=FUENTE, width=20, command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=2)

# Juego (Tablero ajustado para caber)
frame_juego = tk.Frame(pantalla_arcade, bg=BG_COLOR)
cuadricula = tk.Frame(frame_juego, bg=BG_COLOR)
cuadricula.pack(pady=5)

botones = [[None for _ in range(3)] for _ in range(3)]
for i in range(3):
    for j in range(3):
        btn = tk.Button(cuadricula, text="", font=('Arial', 14, 'bold'), width=3, height=1,
                        command=lambda f=i, c=j: click_boton(f, c))
        btn.grid(row=i, column=j, padx=2, pady=2)
        botones[i][j] = btn

tk.Button(frame_juego, text="Abandonar", font=FUENTE, bg="lightgray", command=lambda: cambiar_pantalla(frame_menu_gato)).pack(pady=5)

# --- INICIO DE LA APLICACIÓN ---
cambiar_pantalla(frame_principal)
ventana.mainloop()