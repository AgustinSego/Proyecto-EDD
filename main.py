import ctypes
import tkinter as tk
from tkinter import messagebox, simpledialog
import random

# =====================================================================
# 1. CONEXIÓN CON EL MOTOR LÓGICO EN C (libgamehub.dll)
# =====================================================================
lib = ctypes.CDLL('./libgamehub.dll')

# --- MAPEO DE GATO ---
TableroGato = (ctypes.c_char * 3) * 3
lib.ganador.argtypes = [ctypes.c_char, TableroGato]
lib.ganador.restype = ctypes.c_int
lib.empate.argtypes = [TableroGato]
lib.empate.restype = ctypes.c_int
lib.movimiento_aleatorio.argtypes = [TableroGato, ctypes.c_char]
lib.mov_minimax.argtypes = [TableroGato, ctypes.c_char]

# --- MAPEO DE CONECTA 4 ---
class JuegoC4(ctypes.Structure):
    _fields_ = [("table", (ctypes.c_char * 7) * 6)]

lib.jugada.argtypes = [ctypes.POINTER(JuegoC4), ctypes.c_int, ctypes.c_char]
lib.jugada.restype = ctypes.c_int
lib.ganaron_c4.argtypes = [ctypes.POINTER(JuegoC4), ctypes.c_char]
lib.ganaron_c4.restype = ctypes.c_int
lib.empate_c4.argtypes = [ctypes.POINTER(JuegoC4)]
lib.empate_c4.restype = ctypes.c_int
lib.maquina_facil.argtypes = [ctypes.POINTER(JuegoC4)]
lib.maquina_facil.restype = ctypes.c_int
lib.maquina_media.argtypes = [ctypes.POINTER(JuegoC4)]
lib.maquina_media.restype = ctypes.c_int
lib.maquina_dificil.argtypes = [ctypes.POINTER(JuegoC4)]
lib.maquina_dificil.restype = ctypes.c_int

# --- MAPEO DE BUSCAMINAS ---
class CasillaBM(ctypes.Structure):
    _fields_ = [("es_mina", ctypes.c_bool), ("revelada", ctypes.c_bool), ("bandera", ctypes.c_bool), ("minas_alrededor", ctypes.c_int)]

class TableroBM(ctypes.Structure):
    _fields_ = [("filas", ctypes.c_int), ("columnas", ctypes.c_int), ("num_minas", ctypes.c_int), ("puntaje", ctypes.c_int),
                ("matriz", ctypes.POINTER(ctypes.POINTER(CasillaBM)))]

lib.crear_tableroBM.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib.crear_tableroBM.restype = ctypes.POINTER(TableroBM)
lib.poner_minas.argtypes = [ctypes.POINTER(TableroBM), ctypes.c_int, ctypes.c_int]
lib.valor_minas_adyacentes.argtypes = [ctypes.POINTER(TableroBM)]
lib.bfs.argtypes = [ctypes.POINTER(TableroBM), ctypes.c_int, ctypes.c_int]
lib.condicion_victoriaBM.argtypes = [ctypes.POINTER(TableroBM)]
lib.condicion_victoriaBM.restype = ctypes.c_bool
lib.liberar_memoriaBM.argtypes = [ctypes.POINTER(TableroBM)]

# =====================================================================
# 2. SISTEMA DE RANKING Y PUNTAJES (Interfaz Python)
# =====================================================================
rankings = {"Gato": [], "Conecta 4": [], "Buscaminas": []}

def registrar_puntaje(juego, puntaje):
    if puntaje > 0:
        nombre = simpledialog.askstring("¡Buen juego!", f"Has ganado {puntaje} pts.\nIngresa tu nombre para el Ranking:")
        if nombre:
            rankings[juego].append({"nombre": nombre, "puntaje": puntaje})
            rankings[juego].sort(key=lambda x: x["puntaje"], reverse=True)

def menu_ranking(frame):
    tk.Label(frame, text="TABLA DE POSICIONES", font=("Arial", 18, "bold")).pack(pady=10)
    for juego, records in rankings.items():
        tk.Label(frame, text=f">>> {juego} <<<", font=("Arial", 10, "bold"), fg="blue").pack(pady=2)
        if not records:
            tk.Label(frame, text="[No hay datos]", font=("Arial", 9)).pack()
        else:
            for i, r in enumerate(records[:3]):
                tk.Label(frame, text=f"{i+1}. {r['nombre']} - {r['puntaje']} pts", font=("Arial", 9)).pack()
    
    tk.Button(frame, text="Volver al Menu Principal", width=20, font=("Arial", 10), command=lambda: cambiar_pantalla(menu_principal)).pack(pady=15)

# =====================================================================
# 3. CONFIGURACIÓN VISUAL MAESTRA
# =====================================================================
ventana = tk.Tk()
ventana.title("GAME HUB - Arcade Edition")
ventana.geometry("800x800")
ventana.configure(bg="black") # Fondo negro por si la imagen no cubre todo

# --- CARGAR IMÁGENES PIXEL ART ---
# --- CARGAR IMÁGENES PIXEL ART ---
try:
    img_arcade = tk.PhotoImage(file="banner.png").zoom(2) # Cambiado a "banner .png"
    img_pantalla = tk.PhotoImage(file="pantalla.png")      # Cambiado a "pantalla.png"
except Exception as e:
    print(f"Error cargando imágenes: {e}")
    img_arcade = None
    img_pantalla = None

# Fondo del gabinete Arcade
lbl_arcade = tk.Label(ventana, image=img_arcade, bg="black")
lbl_arcade.place(relx=0.5, rely=0.5, anchor="center")

# =====================================================================
# ZONA DE CALIBRACIÓN DE LA PANTALLA AZUL
# Ajusta estos números para que encaje perfecto en el cristal de tu dibujo
# =====================================================================
ANCHO_PANTALLA = 370  
ALTO_PANTALLA = 270   
EJE_X_REL = 0.51       
EJE_Y_REL = 0.5

# Este Frame invisible es el contenedor estricto de tu pantalla
pantalla_arcade = tk.Frame(ventana, width=ANCHO_PANTALLA, height=ALTO_PANTALLA, bg="black")
pantalla_arcade.place(relx=EJE_X_REL, rely=EJE_Y_REL, anchor="center")
pantalla_arcade.pack_propagate(False) # Evita que la pantalla cambie de tamaño

frame_actual = None

def cambiar_pantalla(nueva_pantalla_func):
    global frame_actual
    if frame_actual is not None:
        frame_actual.destroy()
    
    # Creamos el nuevo frame dentro de la pantalla del arcade
    frame_actual = tk.Frame(pantalla_arcade, bg="black")
    frame_actual.place(relwidth=1, relheight=1)
    
    # Pintamos las rayas azules en el fondo del frame
    if img_pantalla:
        fondo_rayas = tk.Label(frame_actual, image=img_pantalla)
        fondo_rayas.place(x=0, y=0, relwidth=1, relheight=1)
        
    nueva_pantalla_func(frame_actual)

# =====================================================================
# 4. MENÚS Y LÓGICA DE JUEGOS (SIN NÚMEROS)
# =====================================================================

# ---------------------------------------------------------------------
# MENÚ PRINCIPAL
# ---------------------------------------------------------------------
def menu_principal(frame):
    tk.Label(frame, text="GAME HUB", font=("Arial", 20, "bold")).pack(pady=15)
    tk.Button(frame, text="Gato", width=15, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_gato)).pack(pady=4)
    tk.Button(frame, text="Conecta 4", width=15, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_c4)).pack(pady=4)
    tk.Button(frame, text="Buscaminas", width=15, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_bm)).pack(pady=4)
    tk.Button(frame, text="Ver Ranking", width=15, font=("Arial", 12), fg="blue", command=lambda: cambiar_pantalla(menu_ranking)).pack(pady=4)
    tk.Button(frame, text="Salir", width=15, font=("Arial", 12), fg="red", command=ventana.quit).pack(pady=10)

# ---------------------------------------------------------------------
# GATO
# ---------------------------------------------------------------------
def menu_gato(frame):
    tk.Label(frame, text="GATO", font=("Arial", 18, "bold")).pack(pady=15)
    tk.Button(frame, text="Jugar PvP", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_gato(f, "pvp", 0))).pack(pady=5)
    tk.Button(frame, text="Jugar PvE", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_gato_pve)).pack(pady=5)
    tk.Button(frame, text="Volver al Menu Principal", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_principal)).pack(pady=20)

def menu_gato_pve(frame):
    tk.Label(frame, text="GATO - PvE", font=("Arial", 18, "bold")).pack(pady=15)
    tk.Button(frame, text="Fácil", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_gato(f, "pve", 1))).pack(pady=5)
    tk.Button(frame, text="Normal", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_gato(f, "pve", 2))).pack(pady=5)
    tk.Button(frame, text="Difícil", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_gato(f, "pve", 3))).pack(pady=5)
    tk.Button(frame, text="Volver al Menu de Gato", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_gato)).pack(pady=20)

def juego_gato(frame, modo, dif):
    tablero_c = TableroGato((b'1', b'2', b'3'), (b'4', b'5', b'6'), (b'7', b'8', b'9'))
    botones = [[None]*3 for _ in range(3)]
    turno = [b'X'] 

    tk.Label(frame, text=f"Gato - {modo.upper()}", font=("Arial", 14, "bold")).pack(pady=5)
    grilla = tk.Frame(frame)
    grilla.pack()

    def sync_ui():
        for i in range(3):
            for j in range(3):
                val = tablero_c[i][j]
                if val in (b'X', b'O'):
                    botones[i][j].config(text=val.decode(), state="disabled")

    def click(f, c):
        if tablero_c[f][c] in (b'X', b'O'): return
        
        tablero_c[f][c] = turno[0]
        sync_ui()

        if lib.ganador(turno[0], tablero_c):
            espacios_vacios = sum(1 for i in range(3) for j in range(3) if tablero_c[i][j] not in (b'X', b'O'))
            if modo == "pvp": pts = 50 + (espacios_vacios * 15)
            elif dif == 1: pts = 250
            elif dif == 2: pts = 500
            else: pts = 1250
            
            messagebox.showinfo("Gato", f"¡Gana {turno[0].decode()}!\nPuntaje: {pts}")
            if turno[0] == b'X': registrar_puntaje("Gato", pts)
            cambiar_pantalla(menu_gato)
            return

        if lib.empate(tablero_c):
            if modo == "pve":
                if dif == 1: pts = 100
                elif dif == 2: pts = 300
                else: pts = 650
                messagebox.showinfo("Gato", f"¡Empate!\nPuntaje: {pts}")
                registrar_puntaje("Gato", pts)
            else:
                messagebox.showinfo("Gato", "¡Empate!")
            cambiar_pantalla(menu_gato)
            return

        if modo == "pvp":
            turno[0] = b'O' if turno[0] == b'X' else b'X'
        else:
            probabilidad = random.randint(1, 100)
            if dif == 1:
                lib.movimiento_aleatorio(tablero_c, b'O')
            elif dif == 2:
                if probabilidad <= 50: lib.movimiento_aleatorio(tablero_c, b'O')
                else: lib.mov_minimax(tablero_c, b'O')
            else:
                if probabilidad <= 10: lib.movimiento_aleatorio(tablero_c, b'O')
                else: lib.mov_minimax(tablero_c, b'O')
            
            sync_ui()
            
            if lib.ganador(b'O', tablero_c):
                messagebox.showinfo("Gato", "¡Gana la CPU!")
                cambiar_pantalla(menu_gato)
                return
            if lib.empate(tablero_c):
                messagebox.showinfo("Gato", "¡Empate!")
                cambiar_pantalla(menu_gato)
                return

    for i in range(3):
        for j in range(3):
            b = tk.Button(grilla, text="", width=3, height=1, font=("Arial", 20), command=lambda f=i, c=j: click(f, c))
            b.grid(row=i, column=j, padx=2, pady=2)
            botones[i][j] = b

    tk.Button(frame, text="Abandonar", fg="red", command=lambda: cambiar_pantalla(menu_gato)).pack(pady=10)

# ---------------------------------------------------------------------
# CONECTA 4
# ---------------------------------------------------------------------
def menu_c4(frame):
    tk.Label(frame, text="CONECTA 4", font=("Arial", 18, "bold")).pack(pady=15)
    tk.Button(frame, text="Jugar PvP", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_c4(f, "pvp", 0))).pack(pady=5)
    tk.Button(frame, text="Jugar PvE", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_c4_pve)).pack(pady=5)
    tk.Button(frame, text="Volver al Menu Principal", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_principal)).pack(pady=20)

def menu_c4_pve(frame):
    tk.Label(frame, text="CONECTA 4 - PvE", font=("Arial", 18, "bold")).pack(pady=15)
    tk.Button(frame, text="Fácil", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_c4(f, "pve", 1))).pack(pady=5)
    tk.Button(frame, text="Medio", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_c4(f, "pve", 2))).pack(pady=5)
    tk.Button(frame, text="Difícil", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(lambda f: juego_c4(f, "pve", 3))).pack(pady=5)
    tk.Button(frame, text="Volver", width=20, font=("Arial", 12), command=lambda: cambiar_pantalla(menu_c4)).pack(pady=20)

def juego_c4(frame, modo, dif):
    tablero = JuegoC4()
    for i in range(6):
        for j in range(7):
            tablero.table[i][j] = b'.'
            
    botones = [[None]*7 for _ in range(6)]
    turno = [b'A']
    fichas_jugadas = [0]

    tk.Label(frame, text=f"Conecta 4 - {modo.upper()}", font=("Arial", 12, "bold")).pack(pady=2)
    grilla = tk.Frame(frame)
    grilla.pack(pady=2)

    def sync_ui():
        for i in range(6):
            for j in range(7):
                val = tablero.table[i][j]
                if val != b'.':
                    color = "red" if val == b'A' else "blue"
                    botones[i][j].config(text="O", fg=color)

    def click(columna):
        if lib.jugada(ctypes.byref(tablero), columna, turno[0]) == 0: return
        fichas_jugadas[0] += 1
        sync_ui()

        if lib.ganaron_c4(ctypes.byref(tablero), turno[0]):
            if modo == "pvp": pts = 100 + ((42 - fichas_jugadas[0]) * 15)
            elif dif == 1: pts = 250
            elif dif == 2: pts = 500
            else: pts = 1000
            
            messagebox.showinfo("C4", f"¡Gana el jugador {turno[0].decode()}!\nPuntaje: {pts}")
            if turno[0] == b'A': registrar_puntaje("Conecta 4", pts)
            cambiar_pantalla(menu_c4)
            return

        if lib.empate_c4(ctypes.byref(tablero)):
            if modo == "pve":
                if dif == 1: pts = 100
                elif dif == 2: pts = 200
                else: pts = 400
                messagebox.showinfo("C4", f"¡Empate!\nPuntaje: {pts}")
                registrar_puntaje("Conecta 4", pts)
            else:
                messagebox.showinfo("C4", "¡Empate!")
            cambiar_pantalla(menu_c4)
            return

        if modo == "pvp":
            turno[0] = b'B' if turno[0] == b'A' else b'A'
        else:
            if dif == 1: col_maq = lib.maquina_facil(ctypes.byref(tablero))
            elif dif == 2: col_maq = lib.maquina_media(ctypes.byref(tablero))
            else: col_maq = lib.maquina_dificil(ctypes.byref(tablero))

            lib.jugada(ctypes.byref(tablero), col_maq, b'B')
            fichas_jugadas[0] += 1
            sync_ui()

            if lib.ganaron_c4(ctypes.byref(tablero), b'B'):
                messagebox.showinfo("C4", "¡Gana la CPU!")
                cambiar_pantalla(menu_c4)
                return
            if lib.empate_c4(ctypes.byref(tablero)):
                messagebox.showinfo("C4", "¡Empate!")
                cambiar_pantalla(menu_c4)
                return

    for i in range(6):
        for j in range(7):
            b = tk.Button(grilla, text=".", width=2, height=1, font=("Arial", 10, "bold"), 
                          command=lambda c=j: click(c))
            b.grid(row=i, column=j, padx=1, pady=1)
            botones[i][j] = b

    tk.Button(frame, text="Abandonar", fg="red", command=lambda: cambiar_pantalla(menu_c4)).pack(pady=5)

# ---------------------------------------------------------------------
# BUSCAMINAS
# ---------------------------------------------------------------------
def menu_bm(frame):
    tk.Label(frame, text="BUSCAMINAS", font=("Arial", 18, "bold")).pack(pady=15)
    tk.Button(frame, text="Fácil (8x8)", width=25, font=("Arial", 11), command=lambda: cambiar_pantalla(lambda f: juego_bm(f, 8, 8, 10))).pack(pady=5)
    tk.Button(frame, text="Moderada (16x16)", width=25, font=("Arial", 11), command=lambda: cambiar_pantalla(lambda f: juego_bm(f, 16, 16, 40))).pack(pady=5)
    tk.Button(frame, text="Difícil (24x24)", width=25, font=("Arial", 11), command=lambda: cambiar_pantalla(lambda f: juego_bm(f, 24, 24, 99))).pack(pady=5)
    tk.Button(frame, text="Volver al Menu Principal", width=25, font=("Arial", 11), command=lambda: cambiar_pantalla(menu_principal)).pack(pady=20)

def juego_bm(frame, filas, columnas, minas):
    tablero_ptr = lib.crear_tableroBM(filas, columnas, minas)
    primer_mov = [True]
    botones = [[None]*columnas for _ in range(filas)]

    tk.Label(frame, text=f"Buscaminas ({filas}x{columnas})", font=("Arial", 12, "bold")).pack(pady=2)
    grilla = tk.Frame(frame)
    grilla.pack()

    def sync_ui(mostrar_minas_al_perder=False):
        for i in range(filas):
            for j in range(columnas):
                casilla = tablero_ptr.contents.matriz[i][j]
                if casilla.revelada or mostrar_minas_al_perder:
                    if casilla.es_mina: botones[i][j].config(text="*", bg="red", state="disabled")
                    elif casilla.minas_alrededor > 0: botones[i][j].config(text=str(casilla.minas_alrededor), bg="lightgray", state="disabled")
                    else: botones[i][j].config(text="", bg="lightgray", state="disabled")
                elif casilla.bandera:
                    botones[i][j].config(text="B", bg="yellow", fg="black")
                else:
                    botones[i][j].config(text="", bg="SystemButtonFace", state="normal")

    def click_izquierdo(event, f, c):
        casilla = tablero_ptr.contents.matriz[f][c]
        if casilla.revelada or casilla.bandera: return

        if primer_mov[0]:
            lib.poner_minas(tablero_ptr, f, c)
            lib.valor_minas_adyacentes(tablero_ptr)
            primer_mov[0] = False

        if casilla.es_mina:
            sync_ui(mostrar_minas_al_perder=True)
            pts_perdida = tablero_ptr.contents.puntaje
            messagebox.showerror("BOOM", f"Pisaste una mina.\nPuntaje parcial: {pts_perdida}")
            registrar_puntaje("Buscaminas", pts_perdida)
            salir_bm()
            return

        if casilla.minas_alrededor == 0: lib.bfs(tablero_ptr, f, c)
        else:
            casilla.revelada = True
            tablero_ptr.contents.puntaje += 10 

        sync_ui()

        if lib.condicion_victoriaBM(tablero_ptr):
            puntaje_final = tablero_ptr.contents.puntaje
            if minas == 10: puntaje_final += 200
            elif minas == 40: puntaje_final += 500
            else: puntaje_final += 1000
            
            sync_ui(mostrar_minas_al_perder=True)
            messagebox.showinfo("Victoria", f"¡Encontraste todas las minas!\nPuntaje final: {puntaje_final}")
            registrar_puntaje("Buscaminas", puntaje_final)
            salir_bm()

    def click_derecho(event, f, c):
        casilla = tablero_ptr.contents.matriz[f][c]
        if casilla.revelada: return
        casilla.bandera = not casilla.bandera
        sync_ui()

    def salir_bm():
        lib.liberar_memoriaBM(tablero_ptr)
        cambiar_pantalla(menu_bm)

    fuente_btn = ("Arial", 8, "bold") if filas > 8 else ("Arial", 10, "bold")
    ancho_btn = 1 if filas >= 16 else 2
    
    for i in range(filas):
        for j in range(columnas):
            b = tk.Button(grilla, width=ancho_btn, height=1, font=fuente_btn)
            b.bind("<Button-1>", lambda event, f=i, c=j: click_izquierdo(event, f, c))
            b.bind("<Button-3>", lambda event, f=i, c=j: click_derecho(event, f, c))
            b.grid(row=i, column=j)
            botones[i][j] = b

    tk.Button(frame, text="Abandonar", fg="red", command=salir_bm).pack(pady=5)

# =====================================================================
# INICIO DEL PROGRAMA
# =====================================================================
cambiar_pantalla(menu_principal)
ventana.mainloop()