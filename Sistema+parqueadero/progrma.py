import time
import datetime
import json
import os
import hashlib
import sqlite3
from enum import Enum, auto
from typing import Dict, Tuple, Optional, List, Any
from collections import deque 


class Config:
    ESTADO_PARQUEADERO_FILE = "estado_parqueadero.json"
    LOG_FILE = "parqueadero_log.txt"
    GRACE_PERIOD_SECONDS = 15 * 60  
    TIPOS_VEHICULOS = ["carro", "moto", "bicicleta"]
    SERVICIOS_LAVADO = {
        "basico": 10.0,
        "completo": 20.0,
        "premium": 35.0
    }
    DB_NAME = "parqueadero.db" 

class TipoVehiculo(Enum):
    CARRO = "carro"
    MOTO = "moto"
    BICICLETA = "bicicleta"


def log_evento(mensaje: str) -> None:
    """Escribe un mensaje con fecha y hora en un archivo de log."""
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(Config.LOG_FILE, "a", encoding='utf-8') as f:
        f.write(f"[{timestamp}] {mensaje}\n")

def mostrar_confirmacion(pregunta: str) -> bool:
    """Muestra una pregunta de confirmación al usuario."""
    while True:
        respuesta = input(f"{pregunta} (s/n): ").strip().lower()
        if respuesta == 's':
            return True
        elif respuesta == 'n':
            return False
        else:
            print("Respuesta inválida. Por favor, ingrese 's' o 'n'.")

def mostrar_progreso(mensaje: str, duracion: int = 2) -> None:
    """Simula una barra de progreso."""
    print(f"\n{mensaje}...", end='', flush=True)
    for _ in range(duracion):
        time.sleep(0.5)
        print(".", end='', flush=True)
    print(" Listo!")



class Usuario:
    """Representa a un usuario del sistema con nombre de usuario, contraseña encriptada y rol."""
    def __init__(self, username: str, password: str, rol: str):
        self.username = username
        self._password = self._encriptar_password(password)
        self.rol = rol  
        self.fecha_creacion = datetime.datetime.now()
        self.ultimo_acceso = None

    def _encriptar_password(self, password: str) -> str:
        """Encripta la contraseña usando SHA256 para almacenamiento seguro."""
        return hashlib.sha256(password.encode()).hexdigest()

    def verificar_password(self, password: str) -> bool:
        """Verifica si la contraseña proporcionada coincide con la almacenada."""
        return self._encriptar_password(password) == self._password

    def actualizar_ultimo_acceso(self):
        """Registra la marca de tiempo del último acceso del usuario."""
        self.ultimo_acceso = datetime.datetime.now()

class SistemaAutenticacion:
    """Maneja el proceso de inicio de sesión y la gestión de usuarios."""
    def __init__(self):
        
        self.usuarios = {
            'admin': Usuario('admin', 'admin123', 'admin'),
            'cliente1': Usuario('cliente1', 'pass123', 'cliente')
        }
        log_evento("Sistema de Autenticación inicializado.")

    def login(self) -> Optional[Usuario]:
        """Gestiona el proceso interactivo de inicio de sesión para un usuario."""
        print("\n" + "="*50)
        print(" INICIO DE SESIÓN ".center(50))
        print("="*50)

        intentos = 3
        while intentos > 0:
            username = input("Usuario: ").strip()
            password = input("Contraseña: ").strip()

            if username in self.usuarios and self.usuarios[username].verificar_password(password):
                usuario = self.usuarios[username]
                usuario.actualizar_ultimo_acceso()
                print(f"\n¡Bienvenido, {usuario.username}!")
                log_evento(f"Inicio de sesión exitoso para {usuario.username} (Rol: {usuario.rol})")
                return usuario

            intentos -= 1
            print(f"\nCredenciales incorrectas. Intentos restantes: {intentos}")
            log_evento(f"Intento de inicio de sesión fallido para {username}")

        print("\nDemasiados intentos fallidos. Saliendo del sistema...")
        return None

class DatabaseManager:
    """Clase para manejar las operaciones de la base de datos SQLite."""
    def __init__(self, db_name: str = Config.DB_NAME):
        self.conn = None
        self.db_name = db_name
        self._connect()
        self._create_tables()

    def _connect(self):
        """Establece la conexión con la base de datos."""
        try:
            self.conn = sqlite3.connect(self.db_name)
            self.cursor = self.conn.cursor()
            log_evento(f"Conexión a la base de datos '{self.db_name}' establecida.")
        except sqlite3.Error as e:
            log_evento(f"Error al conectar a la base de datos: {e}")
            print(f"Error al conectar a la base de datos: {e}")
            exit()

    def _create_tables(self):
        """Crea las tablas necesarias si no existen."""
        try:
            self.cursor.execute('''
                CREATE TABLE IF NOT EXISTS vehiculos (
                    identificador TEXT PRIMARY KEY,
                    hora_entrada REAL,
                    fila INTEGER,
                    columna INTEGER,
                    tipo_vehiculo TEXT,
                    servicio_lavado TEXT
                )
            ''')
            self.conn.commit()
            log_evento("Tabla 'vehiculos' verificada/creada.")
        except sqlite3.Error as e:
            log_evento(f"Error al crear tablas: {e}")
            print(f"Error al crear tablas: {e}")

    def guardar_vehiculo(self, identificador: str, data: Tuple[float, Tuple[int, int], str, Optional[str]]):
        """Guarda o actualiza la información de un vehículo en la base de datos."""
        hora_entrada, (fila, columna), tipo_vehiculo, servicio_lavado = data
        try:
            self.cursor.execute('''
                INSERT OR REPLACE INTO vehiculos (identificador, hora_entrada, fila, columna, tipo_vehiculo, servicio_lavado)
                VALUES (?, ?, ?, ?, ?, ?)
            ''', (identificador, hora_entrada, fila, columna, tipo_vehiculo, servicio_lavado))
            self.conn.commit()
            log_evento(f"Vehículo '{identificador}' guardado/actualizado en DB.")
        except sqlite3.Error as e:
            log_evento(f"Error al guardar vehículo '{identificador}': {e}")

    def eliminar_vehiculo(self, identificador: str):
        """Elimina un vehículo de la base de datos."""
        try:
            self.cursor.execute("DELETE FROM vehiculos WHERE identificador = ?", (identificador,))
            self.conn.commit()
            log_evento(f"Vehículo '{identificador}' eliminado de la DB.")
        except sqlite3.Error as e:
            log_evento(f"Error al eliminar vehículo '{identificador}': {e}")

    def cargar_vehiculos(self) -> Dict[str, Tuple[float, Tuple[int, int], str, Optional[str]]]:
        """Carga todos los vehículos de la base de datos."""
        try:
            self.cursor.execute("SELECT identificador, hora_entrada, fila, columna, tipo_vehiculo, servicio_lavado FROM vehiculos")
            vehiculos_data = self.cursor.fetchall()
            ocupacion = {}
            for row in vehiculos_data:
                identificador, hora_entrada, fila, columna, tipo_vehiculo, servicio_lavado = row
                ocupacion[identificador] = (hora_entrada, (fila, columna), tipo_vehiculo, servicio_lavado)
            log_evento("Vehículos cargados desde la base de datos.")
            return ocupacion
        except sqlite3.Error as e:
            log_evento(f"Error al cargar vehículos: {e}")
            return {}

    def close(self):
        """Cierra la conexión a la base de datos."""
        if self.conn:
            self.conn.close()
            log_evento(f"Conexión a la base de datos '{self.db_name}' cerrada.")

class Historial:
    """Clase para registrar eventos importantes en el historial del sistema."""
    def __init__(self):
        log_evento("Módulo de Historial inicializado.")

    def registrar_evento(self, evento_tipo: str, detalles: Dict[str, Any]) -> None:
        """Registra un evento con detalles en el log."""
        detalle_str = ", ".join([f"{k}: {v}" for k, v in detalles.items()])
        log_evento(f"HISTORIAL - {evento_tipo}: {detalle_str}")

class ReportGenerator:
    """Genera varios reportes sobre el estado del parqueadero y finanzas."""
    @staticmethod
    def generar_reporte_ocupacion(parqueadero: Any) -> None:
        """Genera un reporte detallado de ocupación del parqueadero."""
        print("\n" + "="*50)
        print(" REPORTE DE OCUPACIÓN ".center(50))
        print("="*50)

        total_ocupados = parqueadero.contar_ocupados()
        capacidad_total = parqueadero.filas * parqueadero.columnas - 2
        total_libres = capacidad_total - total_ocupados

        print(f"\nCapacidad Total Real: {capacidad_total}")
        print(f"Ocupados: {total_ocupados} ({total_ocupados/capacidad_total:.1%})" if capacidad_total > 0 else "0%")
        print(f"Libres: {total_libres} ({total_libres/capacidad_total:.1%})" if capacidad_total > 0 else "0%")

        print("\nOcupación por Zona:")
        for tipo, zona in parqueadero.zonas.items():
            ocupados = sum(1 for f in range(zona['start_row'], zona['end_row']+1)
                           for c in range(parqueadero.columnas)
                           if parqueadero.mapa[f][c] is not None and (f,c) != parqueadero.entrada and (f,c) != parqueadero.salida)
            total = (zona['end_row'] - zona['start_row'] + 1) * parqueadero.columnas
            if parqueadero.entrada[0] >= zona['start_row'] and parqueadero.entrada[0] <= zona['end_row'] and \
               parqueadero.entrada[1] >= 0 and parqueadero.entrada[1] < parqueadero.columnas:
                total -= 1
            if parqueadero.salida[0] >= zona['start_row'] and parqueadero.salida[0] <= zona['end_row'] and \
               parqueadero.salida[1] >= 0 and parqueadero.salida[1] < parqueadero.columnas and \
               parqueadero.salida != parqueadero.entrada:
                total -= 1
            total = max(0, total)

            print(f"- {zona['display_name']}: {ocupados}/{total} ({ocupados/total:.1%})" if total > 0 else f"- {zona['display_name']}: 0/0 (0.0%)")

        print("\nVehículos Estacionados Actualmente:")
        if not parqueadero.ocupacion:
            print("No hay vehículos estacionados.")
        else:
            for id_veh, (hora_entrada, _, tipo, _) in parqueadero.ocupacion.items():
                horas = (time.time() - hora_entrada) / 3600
                print(f"- {id_veh} ({tipo.capitalize()}): {horas:.1f} horas")

        print("="*50)
        log_evento("Reporte de ocupación generado.")

    @staticmethod
    def generar_reporte_financiero() -> None:
        """Genera un reporte financiero basado en el log de eventos."""
        try:
            with open(Config.LOG_FILE, 'r', encoding='utf-8') as f:
                lineas = f.readlines()

            ingresos = 0.0
            lavados_solicitados = 0
            vehiculos_retirados = 0
            total_horas_estacionado = 0.0

            for linea in lineas:
                if "Vehículo retirado. Costo Final:" in linea:
                    partes = linea.split("Costo Final: $")
                    if len(partes) > 1:
                        try:
                            monto = float(partes[1].split()[0])
                            ingresos += monto
                            vehiculos_retirados += 1
                            if "Tiempo estacionado:" in linea:
                                tiempo_partes = linea.split("Tiempo estacionado: ")
                                if len(tiempo_partes) > 1:
                                    horas_str = tiempo_partes[1].split(" horas")[0]
                                    total_horas_estacionado += float(horas_str)
                        except (IndexError, ValueError):
                            continue
                elif "solicitó lavado" in linea:
                    lavados_solicitados += 1

            print("\n" + "="*50)
            print(" REPORTE FINANCIERO ".center(50))
            print("="*50)
            print(f"\nTotal Ingresos: ${ingresos:.2f}")
            print(f"Vehículos retirados: {vehiculos_retirados}")
            print(f"Servicios de lavado solicitados: {lavados_solicitados}")
            print(f"Promedio por vehículo: ${ingresos/vehiculos_retirados:.2f}" if vehiculos_retirados > 0 else "Promedio por vehículo: $0.00")
            print(f"Total de horas estacionado (aprox): {total_horas_estacionado:.2f} horas")
            print("="*50)
            log_evento("Reporte financiero generado.")

        except FileNotFoundError:
            print("\nNo se encontró el archivo de log para generar reportes.")
            log_evento("Error: Archivo de log no encontrado para reporte financiero.")
        except Exception as e:
            print(f"\nError al generar reporte financiero: {e}")
            log_evento(f"Error al generar reporte financiero: {e}")


class SistemaReservas:
    """Gestiona las reservas de espacios de parqueo."""
    def __init__(self, parqueadero: Any):
        self.parqueadero = parqueadero
        self.reservas = {}
        self.next_reserva_id = 1
        log_evento("Sistema de Reservas inicializado.")

    def hacer_reserva(self, usuario: Usuario) -> None:
        """Permite a los usuarios (admins) reservar espacios."""
        if usuario.rol != 'admin':
            print("\nSolo los administradores pueden hacer reservas.")
            log_evento(f"Intento de reserva de '{usuario.username}' (no admin).")
            return

        print("\n" + "="*50)
        print(" SISTEMA DE RESERVAS ".center(50))
        print("="*50)

        tipo = input("Tipo de vehículo (carro/moto/bicicleta): ").strip().lower()
        if tipo not in Config.TIPOS_VEHICULOS:
            print("Tipo de vehículo inválido.")
            return

        id_vehiculo = input("Placa/ID del vehículo: ").strip().upper()
        if not id_vehiculo:
            print("El ID del vehículo no puede estar vacío.")
            return

        print("\nEspacios disponibles:")
        self.parqueadero.mostrar_mapa()

        try:
            fila = int(input(f"Fila para reservar (1-{self.parqueadero.filas}): ")) - 1
            columna = int(input(f"Columna para reservar (1-{self.parqueadero.columnas}): ")) - 1

            if not (0 <= fila < self.parqueadero.filas and 0 <= columna < self.parqueadero.columnas):
                print("Ubicación inválida.")
                return

            if self.parqueadero.mapa[fila][columna] is not None and \
               not str(self.parqueadero.mapa[fila][columna]).startswith("R-"):
                print("Este espacio ya está ocupado.")
                return

            for res_id, (res_veh, _, res_fila, res_col, _) in self.reservas.items():
                if res_fila == fila and res_col == columna:
                    print(f"Este espacio ya está reservado por el vehículo {res_veh} (Reserva {res_id}).")
                    return

            zona = self.parqueadero.zonas.get(tipo, {})
            if not (zona.get('start_row', 0) <= fila <= zona.get('end_row', 0)):
                print(f"Este espacio no corresponde a la zona del vehículo ({tipo.capitalize()}).")
                return
            
            
            if not self.parqueadero._find_path(self.parqueadero.entrada, (fila, columna), id_vehiculo):
                print(f"Error: El espacio en Fila {fila+1}, Columna {columna+1} no es accesible desde la entrada.")
                return

            reserva_id = f"RES-{self.next_reserva_id:04d}"
            self.next_reserva_id += 1
            self.reservas[reserva_id] = (id_vehiculo, tipo, fila, columna, time.time())
            self.parqueadero.mapa[fila][columna] = f"R-{id_vehiculo[:5]}"
            log_evento(f"Reserva {reserva_id} creada para {id_vehiculo} en ({fila+1},{columna+1}).")

            print(f"\n✓ Reserva {reserva_id} creada exitosamente")
            print(f"Vehículo: {id_vehiculo} ({tipo.capitalize()})")
            print(f"Ubicación: Fila {fila+1}, Columna {columna+1}")
            print(f"Hora reserva: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")

        except ValueError:
            print("Por favor ingrese números válidos.")
        except Exception as e:
            print(f"Ocurrió un error al hacer la reserva: {e}")
            log_evento(f"Error al hacer reserva: {e}")

    def ver_reservas(self) -> None:
        """Muestra todas las reservas activas."""
        print("\n" + "="*50)
        print(" RESERVAS ACTIVAS ".center(50))
        print("="*50)

        if not self.reservas:
            print("\nNo hay reservas activas.")
            return

        for reserva_id, (id_veh, tipo, fila, col, hora) in self.reservas.items():
            tiempo_transcurrido = (time.time() - hora) / 60
            print(f"\nID Reserva: {reserva_id}")
            print(f"Vehículo: {id_veh} ({tipo.capitalize()})")
            print(f"Ubicación: Fila {fila+1}, Columna {col+1}")
            print(f"Tiempo desde reserva: {tiempo_transcurrido:.1f} minutos")

        print("="*50)
        log_evento("Reporte de reservas activas generado.")

    def verificar_y_aplicar_reserva(self, id_vehiculo: str, fila: int, columna: int, tipo_vehiculo: str) -> bool:
        """Verifica si el estacionamiento actual coincide con una reserva y la elimina."""
        for res_id, (res_veh, res_tipo, res_fila, res_col, _) in list(self.reservas.items()):
            if res_veh == id_vehiculo and res_tipo == tipo_vehiculo and res_fila == fila and res_col == columna:
                del self.reservas[res_id]
                log_evento(f"Reserva {res_id} para {id_vehiculo} aplicada y eliminada.")
                print(f"\n¡Reserva {res_id} para {id_vehiculo} aplicada exitosamente!")
                if self.parqueadero.mapa[fila][columna] == f"R-{id_vehiculo[:5]}":
                     self.parqueadero.mapa[fila][columna] = None
                return True
        return False

class Tarifador:
    """Calcula las tarifas de parqueo considerando factores dinámicos."""
    def __init__(self):
        self.tarifas_base = {
            TipoVehiculo.CARRO.value: 5.0,
            TipoVehiculo.MOTO.value: 3.0,
            TipoVehiculo.BICICLETA.value: 1.0
        }
        self.factores_dinamicos = {
            'hora_pico': 1.5,
            'fin_de_semana': 1.3,
            'evento_especial': 2.0
        }
        self.horas_pico = [(7, 9), (17, 19)]
        self.eventos_especiales = []
        log_evento("Tarifador inicializado.")

    def agregar_evento(self, fecha: datetime.date, descripcion: str, usuario: Usuario) -> None:
        """Registra un evento especial para tarifas dinámicas."""
        if usuario.rol != 'admin':
            print("\nSolo los administradores pueden añadir eventos especiales.")
            log_evento(f"Intento de añadir evento de '{usuario.username}' (no admin).")
            return

        self.eventos_especiales.append((fecha, descripcion))
        log_evento(f"Evento especial agregado: {descripcion} el {fecha} por {usuario.username}.")

    def calcular_tarifa(self, tipo_vehiculo: str, hora_entrada: float, hora_salida: float) -> float:
        """Calcula la tarifa considerando factores dinámicos."""
        tarifa_base = self.tarifas_base.get(tipo_vehiculo, 0)
        if tarifa_base == 0:
            return 0.0

        tiempo_horas = (hora_salida - hora_entrada) / 3600

        factor = 1.0
        fecha_entrada = datetime.datetime.fromtimestamp(hora_entrada)

        if fecha_entrada.weekday() >= 5:
            factor *= self.factores_dinamicos['fin_de_semana']

        hora = fecha_entrada.hour
        for inicio, fin in self.horas_pico:
            if inicio <= hora < fin:
                factor *= self.factores_dinamicos['hora_pico']
                break

        fecha_sin_hora = fecha_entrada.date()
        for evento_fecha, _ in self.eventos_especiales:
            if evento_fecha == fecha_sin_hora:
                factor *= self.factores_dinamicos['evento_especial']
                break
        
        return tarifa_base * tiempo_horas * factor

    def mostrar_tarifas_actuales(self) -> None:
        """Muestra las tarifas considerando los factores actuales."""
        ahora = time.time()
        fecha = datetime.datetime.now()

        print("\n" + "="*50)
        print(" TARIFAS ACTUALES ".center(50))
        print("="*50)

        factores = []
        if fecha.weekday() >= 5:
            factores.append(f"Fin de semana (+{int((self.factores_dinamicos['fin_de_semana']-1)*100)}%)")

        hora_actual = fecha.hour
        for inicio, fin in self.horas_pico:
            if inicio <= hora_actual < fin:
                factores.append(f"Hora pico (+{int((self.factores_dinamicos['hora_pico']-1)*100)}%)")
                break

        fecha_sin_hora = fecha.date()
        for evento_fecha, desc in self.eventos_especiales:
            if evento_fecha == fecha_sin_hora:
                factores.append(f"Evento especial: {desc} (+{int((self.factores_dinamicos['evento_especial']-1)*100)}%)")
                break

        if factores:
            print("\nFactores aplicables ahora:")
            for f in factores:
                print(f"- {f}")
        else:
            print("\nTarifas base normales aplicando.")

        print("\nTarifas por hora (aproximado con factores actuales):")
        for tipo_enum in TipoVehiculo:
            tipo = tipo_enum.value
            tarifa = self.tarifas_base.get(tipo, 0.0)
            tarifa_actual = tarifa
            
            factor_actual = 1.0
            if fecha.weekday() >= 5:
                factor_actual *= self.factores_dinamicos['fin_de_semana']
            for inicio, fin in self.horas_pico:
                if inicio <= hora_actual < fin:
                    factor_actual *= self.factores_dinamicos['hora_pico']
                    break
            for evento_fecha, _ in self.eventos_especiales:
                if evento_fecha == fecha_sin_hora:
                    factor_actual *= self.factores_dinamicos['evento_especial']
                    break

            tarifa_actual = tarifa * factor_actual
            print(f"- {tipo.capitalize()}: ${tarifa_actual:.2f} (Base: ${tarifa:.2f})")

        print("="*50)
        log_evento("Tarifas actuales mostradas.")


class Factura:
    """Representa una factura de parqueo con todos los detalles de costos y tiempo."""
    def __init__(self, identificador_vehiculo: str, tipo_vehiculo: str,
                 tiempo_estacionado_segundos: float, costo_parqueo_original: float,
                 valor_descuento_lavado: float, costo_lavado: float,
                 costo_total_final: float, fecha_hora_salida: datetime.datetime):
        self.identificador_vehiculo = identificador_vehiculo
        self.tipo_vehiculo = tipo_vehiculo
        self.tiempo_estacionado_segundos = tiempo_estacionado_segundos
        self.costo_parqueo_original = costo_parqueo_original
        self.valor_descuento_lavado = valor_descuento_lavado
        self.costo_lavado = costo_lavado
        self.costo_total_final = costo_total_final
        self.fecha_hora_salida = fecha_hora_salida
        self.id_factura = self._generar_id_factura()
        log_evento(f"Factura {self.id_factura} creada para {self.identificador_vehiculo}.")


    def _generar_id_factura(self) -> str:
        """Genera un ID único para la factura basado en la fecha y hora actual."""
        return datetime.datetime.now().strftime("FACT-%Y%m%d%H%M%S")

    def mostrar_factura(self) -> None:
        """Muestra los detalles de la factura de forma clara y formateada."""
        tiempo_minutos = self.tiempo_estacionado_segundos / 60
        tiempo_horas = self.tiempo_estacionado_segundos / 3600

        print("\n" + "="*40)
        print("FACTURA DE PARQUEADERO".center(40))
        print("="*40)
        print(f"{'ID Factura:':<20} {self.id_factura}")
        print(f"{'Fecha/Hora Salida:':<20} {self.fecha_hora_salida.strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"{'Vehículo:':<20} {self.identificador_vehiculo} ({self.tipo_vehiculo.capitalize()})")
        print("\nDetalles de Tiempo:")
        print(f"{'Segundos:':<15} {self.tiempo_estacionado_segundos:.2f}")
        print(f"{'Minutos:':<15} {tiempo_minutos:.2f}")
        print(f"{'Horas:':<15} {tiempo_horas:.2f}")

        print("\nDetalles de Costos:")
        print(f"{'Costo Parqueo Base:':<20} ${self.costo_parqueo_original:.2f}")

        if self.valor_descuento_lavado > 0:
            print(f"{'Descuento Lavado:':<20} -${self.valor_descuento_lavado:.2f}")

        if self.costo_lavado > 0:
            print(f"{'Costo Lavado:':<20} +${self.costo_lavado:.2f}")

        print("-"*40)
        print(f"{'TOTAL A PAGAR:':<20} ${self.costo_total_final:.2f}")
        print("="*40)

        log_evento(f"Factura {self.id_factura} mostrada. Costo Final: ${self.costo_total_final:.2f}, Tiempo estacionado: {tiempo_horas:.2f} horas.")


class Parqueadero:
    """Clase base para la gestión del parqueadero, incluyendo el mapa y la lógica de zonas."""
    def __init__(self, filas: int, columnas: int):
        self.filas = filas
        self.columnas = columnas
        self.entrada = (0, 0) 
        self.salida = (self.filas - 1, self.columnas - 1) 
        self.next_bicycle_id = 1
        self.descuento_lavado = 0.30

        self._configurar_zonas()
        self._inicializar_mapa()
        self.ocupacion: Dict[str, Tuple[float, Tuple[int, int], str, Optional[str]]] = {}
        log_evento(f"Parqueadero de {filas}x{columnas} inicializado en memoria.")

    def _configurar_zonas(self) -> None:
        """Configura las zonas del parqueadero según porcentajes predefinidos."""
        num_filas_carro = max(1, int(self.filas * 0.5))
        num_filas_moto = max(1, int(self.filas * 0.3))
        num_filas_bicicleta = self.filas - num_filas_carro - num_filas_moto
        if num_filas_bicicleta < 1:
            num_filas_bicicleta = 1
            if (num_filas_carro + num_filas_moto + num_filas_bicicleta) > self.filas:
                exceso = (num_filas_carro + num_filas_moto + num_filas_bicicleta) - self.filas
                if num_filas_moto > 1:
                    num_filas_moto = max(1, num_filas_moto - exceso)
                elif num_filas_carro > 1:
                    num_filas_carro = max(1, num_filas_carro - exceso)


        self.zonas = {
            TipoVehiculo.CARRO.value: {
                "start_row": 0,
                "end_row": num_filas_carro - 1,
                "display_name": "Zona de Carros"
            },
            TipoVehiculo.MOTO.value: {
                "start_row": num_filas_carro,
                "end_row": num_filas_carro + num_filas_moto - 1,
                "display_name": "Zona de Motos"
            },
            TipoVehiculo.BICICLETA.value: {
                "start_row": num_filas_carro + num_filas_moto,
                "end_row": num_filas_carro + num_filas_moto + num_filas_bicicleta - 1,
                "display_name": "Zona de Bicicletas"
            }
        }

        for zone in self.zonas.values():
            zone["start_row"] = max(0, zone["start_row"])
            zone["end_row"] = min(zone["end_row"], self.filas - 1)
        
        if self.filas < 3:
            print("Advertencia: Demasiado pocas filas para zonas diferenciadas. Pueden superponerse.")


    def _inicializar_mapa(self) -> None:
        """Inicializa el mapa del parqueadero con espacios libres y una red de rutas detallada."""
        self.mapa = [[None for _ in range(self.columnas)] for _ in range(self.filas)]

        route_columns = [i for i in range(self.columnas) if i % 2 == 0] 

        for r in range(self.filas):
            for c in range(self.columnas):
                if c in route_columns:
                    self.mapa[r][c] = "RUTA"
                else:
                    self.mapa[r][c] = None 

        if self.mapa[self.entrada[0]][self.entrada[1]] is None:
            self.mapa[self.entrada[0]][self.entrada[1]] = "RUTA"
        self.mapa[self.entrada[0]][self.entrada[1]] = "Entrada"

        if self.mapa[self.salida[0]][self.salida[1]] is None:
            self.mapa[self.salida[0]][self.salida[1]] = "RUTA"
        if self.salida != self.entrada:
            self.mapa[self.salida[0]][self.salida[1]] = "Salida"
        
        for c in range(self.columnas):
            if (0, c) != self.entrada: 
                self.mapa[0][c] = "RUTA"

        log_evento("Mapa del parqueadero inicializado con ruta detallada.")


    def _find_path(self, start: Tuple[int, int], end: Tuple[int, int], current_vehicle_id: Optional[str] = None) -> Optional[List[Tuple[int, int]]]:
        """
        Encuentra una ruta desde el punto de inicio hasta el punto final usando BFS.
        Considera celdas vacías (None), celdas 'RUTA' y la celda final como transitables.
        Evita celdas ocupadas por otros vehículos o reservas para otros vehículos.
        `current_vehicle_id` es el ID del vehículo que intenta estacionar, para permitirle
        pasar por su propia reserva si ya existe.
        """
        rows, cols = self.filas, self.columnas
        queue = deque([(start, [start])])
        visited = {start}

        while queue:
            (r, c), path = queue.popleft()

            if (r, c) == end:
                return path

            directions = [(0, 1), (0, -1), (1, 0), (-1, 0)] 

            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                next_pos = (nr, nc)

                if not (0 <= nr < rows and 0 <= nc < cols): 
                    continue

                if next_pos in visited: 
                    continue

                cell_content = self.mapa[nr][nc]
                is_target_cell = (nr, nc) == end

                is_traversable = False

                if cell_content is None: 
                    is_traversable = True
                elif cell_content == "RUTA": 
                    is_traversable = True
                elif is_target_cell:
                    is_traversable = True
                elif cell_content == "Entrada" or cell_content == "Salida": 
                    is_traversable = True
                elif str(cell_content).startswith("R-"): 
                    if current_vehicle_id and cell_content == f"R-{current_vehicle_id[:5]}":
                        is_traversable = True
                
                if (cell_content is not None and
                    cell_content != "RUTA" and
                    cell_content != "Entrada" and
                    cell_content != "Salida" and
                    not (str(cell_content).startswith("R-") and current_vehicle_id and cell_content == f"R-{current_vehicle_id[:5]}") and
                    not is_target_cell):
                    is_traversable = False


                if is_traversable:
                    visited.add(next_pos)
                    queue.append((next_pos, path + [next_pos]))

        return None 

    def guardar_estado(self, nombre_archivo: str) -> None:
        """Guarda el estado actual del parqueadero en un archivo JSON (solo para compatibilidad)."""
        log_evento("Advertencia: 'guardar_estado' llamado en Parqueadero base. Usar ParqueaderoDB para persistencia.")
        pass

    def cargar_estado(self, nombre_archivo: str) -> bool:
        """Carga el estado del parqueadero desde un archivo JSON (solo para compatibilidad)."""
        log_evento("Advertencia: 'cargar_estado' llamado en Parqueadero base. Usar ParqueaderoDB para persistencia.")
        return False

    def buscar_vehiculo(self, identificador: str) -> Optional[Tuple[int, int]]:
        """Busca un vehículo por su identificador y muestra su ubicación."""
        if identificador in self.ocupacion:
            hora_entrada, (fila, columna), tipo_vehiculo, servicio_lavado = self.ocupacion[identificador]
            tiempo_transcurrido = time.time() - hora_entrada
            print("\n" + "="*40)
            print("INFORMACIÓN DEL VEHÍCULO".center(40))
            print("="*40)
            print(f"{'Identificador:':<20} {identificador}")
            print(f"{'Tipo:':<20} {tipo_vehiculo.capitalize()}")
            print(f"{'Ubicación:':<20} Fila {fila + 1}, Columna {columna + 1}")
            print(f"{'Tiempo estacionado:':<20} {tiempo_transcurrido/60:.2f} minutos")
            print(f"{'Servicio de Lavado:':<20} {servicio_lavado.capitalize() if servicio_lavado else 'No'}")
            print("="*40)
            log_evento(f"Vehículo '{identificador}' buscado. Ubicado en Fila {fila + 1}, Columna {columna + 1}. Lavado: {servicio_lavado or 'No'}.")
            return (fila, columna)

        print(f"\nEl vehículo con identificador '{identificador}' no se encuentra estacionado.")
        log_evento(f"Intento de búsqueda fallido para '{identificador}'. No encontrado.")
        return None

    def mostrar_mapa(self, path_to_highlight: Optional[List[Tuple[int, int]]] = None) -> None:
        """Muestra el mapa del parqueadero con colores y formato mejorado, y opcionalmente una ruta."""
        print("\n" + "═" * (self.columnas * 12 + 10))
        print("MAPA DEL PARQUEADERO".center(self.columnas * 12 + 10))
        print("═" * (self.columnas * 12 + 10))

        headers = ["Fila/Col"] + [f"Col {c+1:^8}" for c in range(self.columnas)]
        print(" | ".join(headers))
        print("-" * (self.columnas * 12 + 10))

        for r in range(self.filas):
            row_display = [f"Fila {r+1:^5}"]
            for c in range(self.columnas):
                content = self._get_cell_content(r, c, path_to_highlight)
                row_display.append(f"{content:^8}")
            print(" | ".join(row_display))

        print("\nLEYENDA:")
        print("- Libre: Espacio disponible")
        print("- ENTRADA: Punto de entrada")
        print("- SALIDA: Punto de salida")
        print("- RUTA: Vía de acceso/circulación")
        print("- [Placa/ID]: Vehículo estacionado")
        print("- R-[Placa/ID]: Espacio Reservado")
        if path_to_highlight:
            print("- RUTA (Vehículo): Ruta sugerida para el vehículo")
        print(f"\nEspacios libres: {self.contar_libres()} | Ocupados: {self.contar_ocupados()}")
        print("="*50)
        log_evento("Mapa del parqueadero mostrado.")


    def _get_cell_content(self, fila: int, columna: int, path_to_highlight: Optional[List[Tuple[int, int]]] = None) -> str:
        """Devuelve el contenido formateado de una celda del mapa, con opción de resaltar ruta."""
        if path_to_highlight and (fila, columna) in path_to_highlight:
            return "\033[95mRUTA\033[0m" 

        if (fila, columna) == self.entrada:
            return "\033[92mENTRADA\033[0m"  
        elif (fila, columna) == self.salida:
            return "\033[91mSALIDA\033[0m"    
        elif self.mapa[fila][columna] is None:
            return "Libre"
        elif self.mapa[fila][columna] == "RUTA":
            return "\033[94mRUTA\033[0m" 
        elif str(self.mapa[fila][columna]).startswith("R-"):
            return f"\033[96m{self.mapa[fila][columna]}\033[0m" 
        else:
            return f"\033[93m{self.mapa[fila][columna]}\033[0m"  

    def contar_libres(self, tipo_vehiculo: Optional[str] = None) -> int:
        """Cuenta el número de espacios libres de parqueo."""
        count = 0
        start_row = 0
        end_row = self.filas - 1

        if tipo_vehiculo and tipo_vehiculo in self.zonas:
            start_row = self.zonas[tipo_vehiculo]["start_row"]
            end_row = self.zonas[tipo_vehiculo]["end_row"]

        for r in range(start_row, end_row + 1):
            for c in range(self.columnas):
            
                if self.mapa[r][c] is None and \
                   (r, c) != self.entrada and (r, c) != self.salida and \
                   self.mapa[r][c] != "RUTA":
                    count += 1
        return count

    def contar_ocupados(self) -> int:
        """Cuenta el número de espacios de parqueo ocupados (excluyendo Entrada/Salida y Rutas)."""
        return len(self.ocupacion)

    def encontrar_libre(self, tipo_vehiculo: str) -> Optional[Tuple[int, int]]:
        """Encuentra el primer espacio libre para el tipo de vehículo especificado."""
        if tipo_vehiculo not in self.zonas:
            return None

        start_row = self.zonas[tipo_vehiculo]["start_row"]
        end_row = self.zonas[tipo_vehiculo]["end_row"]

        for r in range(start_row, end_row + 1):
            for c in range(self.columnas):
                if self.mapa[r][c] is None and \
                   (r, c) != self.entrada and (r, c) != self.salida and \
                   self.mapa[r][c] != "RUTA":
                    return r, c
        return None

    def estacionar_vehiculo_interactivo(self, sistema_reservas: Any) -> bool:
        """Interfaz interactiva para estacionar un vehículo."""
        print("\n" + "="*40)
        print("ESTACIONAR VEHÍCULO".center(40))
        print("="*40)

        tipo_vehiculo_str = self._solicitar_tipo_vehiculo()
        if not tipo_vehiculo_str:
            return False
        
        tipo_vehiculo = TipoVehiculo(tipo_vehiculo_str)

        placa = ""
        if tipo_vehiculo != TipoVehiculo.BICICLETA:
            placa = input("Ingrese la placa del vehículo (6 caracteres): ").strip().upper()
            if len(placa) != 6:
                print("Error: La placa debe tener exactamente 6 caracteres.")
                return False
            if placa in self.ocupacion:
                print(f"Error: El vehículo con placa '{placa}' ya está estacionado.")
                return False
        
        identificador = self._generar_identificador(placa, tipo_vehiculo)
        
        ubicacion_auto = mostrar_confirmacion("¿Desea asignación automática de espacio?")

        if ubicacion_auto:
            return self._estacionar_automatico(identificador, tipo_vehiculo, sistema_reservas)
        else:
            return self._estacionar_manual(identificador, tipo_vehiculo, sistema_reservas)

    def _solicitar_tipo_vehiculo(self) -> Optional[str]:
        """Solicita y valida el tipo de vehículo."""
        while True:
            tipo = input(f"Tipo de vehículo ({'/'.join(Config.TIPOS_VEHICULOS)}): ").strip().lower()
            if tipo in Config.TIPOS_VEHICULOS:
                return tipo
            print(f"Tipo inválido. Opciones: {', '.join(Config.TIPOS_VEHICULOS)}")

    def _estacionar_automatico(self, identificador: str, tipo_vehiculo: TipoVehiculo, sistema_reservas: Any) -> bool:
        """Estaciona el vehículo en el primer espacio disponible."""
        spot = self.encontrar_libre(tipo_vehiculo.value)
        if not spot:
            print(f"No hay espacios libres disponibles para {tipo_vehiculo.value}.")
            return False

        fila, columna = spot
        
        path = self._find_path(self.entrada, spot, identificador)
        if not path:
            print(f"Error: No se encontró una ruta accesible para {identificador} a Fila {fila+1}, Columna {columna+1}.")
            log_evento(f"Error: No se encontró ruta para {identificador} a ({fila+1},{columna+1}).")
            return False

        print("\n--- Ruta sugerida para el vehículo ---")
        self.mostrar_mapa(path_to_highlight=path)
        print("------------------------------------")
        
        sistema_reservas.verificar_y_aplicar_reserva(identificador, fila, columna, tipo_vehiculo.value)
        servicio_lavado = self._solicitar_servicio_lavado(identificador, tipo_vehiculo.value)

        return self._asignar_espacio(identificador, tipo_vehiculo.value, fila, columna, servicio_lavado)

    def _estacionar_manual(self, identificador: str, tipo_vehiculo: TipoVehiculo, sistema_reservas: Any) -> bool:
        """Estaciona el vehículo en la ubicación especificada por el usuario."""
        while True:
            try:
                fila = int(input(f"Ingrese fila (1-{self.filas}): ")) - 1
                columna = int(input(f"Ingrese columna (1-{self.columnas}): ")) - 1

                if not (0 <= fila < self.filas and 0 <= columna < self.columnas):
                    print(f"Rango inválido. Fila: 1-{self.filas}, Columna: 1-{self.columnas}")
                    continue
                
                if self.mapa[fila][columna] == "RUTA":
                    print(f"Error: No se puede estacionar en una vía de circulación (RUTA).")
                    continue

                if not (self.zonas[tipo_vehiculo.value]["start_row"] <= fila <= self.zonas[tipo_vehiculo.value]["end_row"]):
                    print(f"Error: La fila {fila+1} no está en la zona designada para {tipo_vehiculo.value.capitalize()}.")
                    continue

                if self.mapa[fila][columna] is not None and \
                   not str(self.mapa[fila][columna]).startswith("R-"):
                    print(f"El espacio en fila {fila+1}, columna {columna+1} ya está ocupado.")
                    continue

                if str(self.mapa[fila][columna]).startswith("R-") and self.mapa[fila][columna] != f"R-{identificador[:5]}":
                     print(f"El espacio en fila {fila+1}, columna {columna+1} está reservado por otro vehículo.")
                     continue

                path = self._find_path(self.entrada, (fila, columna), identificador)
                if not path:
                    print(f"Error: No se encontró una ruta accesible para {identificador} a Fila {fila+1}, Columna {columna+1}.")
                    log_evento(f"Error: No se encontró ruta para {identificador} a ({fila+1},{columna+1}).")
                    continue 

                print("\n--- Ruta sugerida para el vehículo ---")
                self.mostrar_mapa(path_to_highlight=path)
                print("------------------------------------")

                sistema_reservas.verificar_y_aplicar_reserva(identificador, fila, columna, tipo_vehiculo.value)

                servicio_lavado = self._solicitar_servicio_lavado(identificador, tipo_vehiculo.value)

                return self._asignar_espacio(identificador, tipo_vehiculo.value, fila, columna, servicio_lavado)

            except ValueError:
                print("Por favor ingrese números válidos.")
            except Exception as e:
                print(f"Ocurrió un error al estacionar manualmente: {e}")
                return False


    def _generar_identificador(self, placa: str, tipo_vehiculo: TipoVehiculo) -> str:
        """Genera el identificador único para el vehículo."""
        if tipo_vehiculo == TipoVehiculo.BICICLETA:
            identificador = f"BICI-{self.next_bicycle_id:03d}"
            self.next_bicycle_id += 1
            return identificador
        return placa

    def _solicitar_servicio_lavado(self, identificador: str, tipo_vehiculo: str) -> Optional[str]:
        """Solicita al usuario si desea servicio de lavado."""
        if tipo_vehiculo not in [TipoVehiculo.CARRO.value, TipoVehiculo.MOTO.value]:
            return None

        if not mostrar_confirmacion(f"¿Desea lavar {identificador} y obtener {self.descuento_lavado*100:.0f}% de descuento?"):
            return None

        print("\nServicios de Lavado Disponibles:")
        for servicio, precio in Config.SERVICIOS_LAVADO.items():
            print(f"- {servicio.capitalize()}: ${precio:.2f}")

        while True:
            servicio = input("Elija tipo de lavado (basico/completo/premium): ").strip().lower()
            if servicio in Config.SERVICIOS_LAVADO:
                log_evento(f"Vehículo {identificador} solicitó lavado tipo: {servicio}.")
                return servicio
            print("Opción inválida. Elija entre basico, completo o premium.")

    def _asignar_espacio(self, identificador: str, tipo_vehiculo: str,
                         fila: int, columna: int, servicio_lavado: Optional[str]) -> bool:
        """Asigna el espacio en el parqueadero al vehículo."""
        if (fila, columna) == self.entrada or (fila, columna) == self.salida:
            print(f"Error: No se puede estacionar en Entrada/Salida.")
            log_evento(f"Intento de estacionar {identificador} en Entrada/Salida ({fila+1},{columna+1}).")
            return False

        if not (self.zonas[tipo_vehiculo]["start_row"] <= fila <= self.zonas[tipo_vehiculo]["end_row"]):
            print(f"Error: La fila {fila+1} no está en la zona designada para {tipo_vehiculo.capitalize()}.")
            log_evento(f"Error: {identificador} intentó estacionar en zona incorrecta ({tipo_vehiculo}) en ({fila+1},{columna+1}).")
            return False

        if self.mapa[fila][columna] is not None and not str(self.mapa[fila][columna]).startswith("R-"):
            print(f"El espacio en fila {fila+1}, columna {columna+1} ya está ocupado por otro vehículo.")
            log_evento(f"Error: {identificador} intentó estacionar en espacio ocupado en ({fila+1},{columna+1}).")
            return False

        if str(self.mapa[fila][columna]).startswith("R-") and self.mapa[fila][columna] != f"R-{identificador[:5]}":
             print(f"El espacio en fila {fila+1}, columna {columna+1} está reservado por otro vehículo.")
             log_evento(f"Error: {identificador} intentó estacionar en espacio reservado por otro en ({fila+1},{columna+1}).")
             return False

        if self.mapa[fila][columna] == "RUTA":
            print(f"Error: No se puede estacionar en una vía de circulación (RUTA).")
            log_evento(f"Error: {identificador} intentó estacionar en una RUTA en ({fila+1},{columna+1}).")
            return False

        self.mapa[fila][columna] = identificador
        self.ocupacion[identificador] = (time.time(), (fila, columna), tipo_vehiculo, servicio_lavado)

        print(f"\n✓ Vehículo {identificador} estacionado en Fila {fila+1}, Columna {columna+1}")
        log_evento(f"Vehículo {identificador} ({tipo_vehiculo}) estacionado en ({fila+1},{columna+1}). Lavado: {servicio_lavado or 'No'}.")
        return True

    def retirar_vehiculo_interactivo(self, tarifador: Tarifador) -> None:
        """Interfaz interactiva para retirar un vehículo."""
        print("\n" + "="*40)
        print("RETIRAR VEHÍCULO".center(40))
        print("="*40)

        identificador = input("Ingrese placa o ID del vehículo: ").strip().upper()

        if identificador not in self.ocupacion:
            print(f"\nEl vehículo '{identificador}' no está en el parqueadero.")
            log_evento(f"Intento de retiro fallido para '{identificador}'. No encontrado.")
            return

        hora_entrada, (fila, columna), tipo_vehiculo, servicio_lavado = self.ocupacion[identificador]

        costo_parqueo_calculado_original = tarifador.calcular_tarifa(
            tipo_vehiculo, hora_entrada, time.time()
        )
        tiempo_estacionado = time.time() - hora_entrada

        costo_parqueo_final = costo_parqueo_calculado_original
        if tiempo_estacionado <= Config.GRACE_PERIOD_SECONDS:
            print("\n¡Período de gracia aplicado! (Menos de 15 minutos)")
            costo_parqueo_final = 0.0

        valor_descuento_lavado = 0.0
        if servicio_lavado and costo_parqueo_final > 0:
            valor_descuento_lavado = costo_parqueo_final * self.descuento_lavado
            costo_parqueo_final -= valor_descuento_lavado
            costo_parqueo_final = max(0.0, costo_parqueo_final)

        costo_lavado = Config.SERVICIOS_LAVADO.get(servicio_lavado, 0.0) if servicio_lavado else 0.0

        costo_total_final = max(0.0, costo_parqueo_final + costo_lavado)

        self._procesar_pago_interactivo(costo_total_final)

        self.db_manager.eliminar_vehiculo(identificador)
        del self.ocupacion[identificador]

        route_columns = [i for i in range(self.columnas) if i % 2 == 0]
        if columna in route_columns and (fila, columna) != self.entrada and (fila, columna) != self.salida:
            self.mapa[fila][columna] = "RUTA"
        else:
            self.mapa[fila][columna] = None 

        log_evento(f"Vehículo {identificador} ({tipo_vehiculo}) retirado de ({fila+1},{columna+1}) y de la DB.")

        factura = self._generar_factura(
            identificador,
            tipo_vehiculo,
            tiempo_estacionado,
            costo_parqueo_original,
            valor_descuento_lavado,
            costo_lavado,
            costo_total_final
        )
        factura.mostrar_factura()


class ParqueaderoDB(Parqueadero):
    """
    Clase que extiende Parqueadero para manejar la persistencia
    del estado de ocupación a través de una base de datos.
    """
    def __init__(self, filas: int, columnas: int, db_manager: DatabaseManager):
        super().__init__(filas, columnas)
        self.db_manager = db_manager
        log_evento("ParqueaderoDB inicializado.")

    def cargar_estado(self, nombre_archivo: str = Config.ESTADO_PARQUEADERO_FILE) -> bool:
        """Carga el estado del parqueadero desde la base de datos."""
        try:
            self.ocupacion = self.db_manager.cargar_vehiculos()
            self._inicializar_mapa() 
            max_bici_id = 0
            for identificador in self.ocupacion.keys():
                if identificador.startswith("BICI-"):
                    try:
                        bici_id = int(identificador.split("-")[1])
                        if bici_id > max_bici_id:
                            max_bici_id = bici_id
                    except ValueError:
                        pass
            self.next_bicycle_id = max_bici_id + 1

            for identificador, (_, (fila, columna), _, _) in self.ocupacion.items():
                if (fila, columna) != self.entrada and (fila, columna) != self.salida:

                    if self.mapa[fila][columna] != "RUTA":
                        self.mapa[fila][columna] = identificador 
                    else:
                        log_evento(f"Advertencia: Vehículo '{identificador}' cargado en una celda RUTA ({fila+1},{columna+1}). Esto puede indicar un error previo en el guardado.")

            log_evento(f"Estado del parqueadero cargado desde la base de datos.")
            return True
        except Exception as e:
            log_evento(f"Error al cargar el estado del parqueadero desde la DB: {e}. Iniciando vacío.")
            self.ocupacion = {}
            self._inicializar_mapa()
            return False

    def _asignar_espacio(self, identificador: str, tipo_vehiculo: str,
                         fila: int, columna: int, servicio_lavado: Optional[str]) -> bool:
        """Asigna el espacio y lo guarda en la base de datos."""
        if super()._asignar_espacio(identificador, tipo_vehiculo, fila, columna, servicio_lavado):
            self.db_manager.guardar_vehiculo(identificador, self.ocupacion[identificador])
            return True
        return False

    def retirar_vehiculo_interactivo(self, tarifador: Tarifador) -> None:
        """Retira el vehículo y lo elimina de la base de datos."""
        print("\n" + "="*40)
        print("RETIRAR VEHÍCULO".center(40))
        print("="*40)

        identificador = input("Ingrese placa o ID del vehículo: ").strip().upper()

        if identificador not in self.ocupacion:
            print(f"\nEl vehículo '{identificador}' no está en el parqueadero.")
            log_evento(f"Intento de retiro fallido para '{identificador}'. No encontrado.")
            return

        hora_entrada, (fila, columna), tipo_vehiculo, servicio_lavado = self.ocupacion[identificador]

        costo_parqueo_calculado_original = tarifador.calcular_tarifa(
            tipo_vehiculo, hora_entrada, time.time()
        )
        tiempo_estacionado = time.time() - hora_entrada

        costo_parqueo_final = costo_parqueo_calculado_original
        if tiempo_estacionado <= Config.GRACE_PERIOD_SECONDS:
            print("\n¡Período de gracia aplicado! (Menos de 15 minutos)")
            costo_parqueo_final = 0.0

        valor_descuento_lavado = 0.0
        if servicio_lavado and costo_parqueo_final > 0:
            valor_descuento_lavado = costo_parqueo_final * self.descuento_lavado
            costo_parqueo_final -= valor_descuento_lavado
            costo_parqueo_final = max(0.0, costo_parqueo_final)

        costo_lavado = Config.SERVICIOS_LAVADO.get(servicio_lavado, 0.0) if servicio_lavado else 0.0

        costo_total_final = max(0.0, costo_parqueo_final + costo_lavado)

        self._procesar_pago_interactivo(costo_total_final)

        self.db_manager.eliminar_vehiculo(identificador)
        del self.ocupacion[identificador]
        
        route_columns = [i for i in range(self.columnas) if i % 2 == 0]
        if columna in route_columns and (fila, columna) != self.entrada and (fila, columna) != self.salida:
            self.mapa[fila][columna] = "RUTA"
        else:
            self.mapa[fila][columna] = None 

        log_evento(f"Vehículo {identificador} ({tipo_vehiculo}) retirado de ({fila+1},{columna+1}) y de la DB.")

        factura = self._generar_factura(
            identificador,
            tipo_vehiculo,
            tiempo_estacionado,
            costo_parqueo_original,
            valor_descuento_lavado,
            costo_lavado,
            costo_total_final
        )
        factura.mostrar_factura()


def mostrar_info_zonas(parqueo: Parqueadero) -> None:
    """Muestra la información de las zonas de parqueo."""
    print("\n" + "="*50)
    print(" ZONAS DE PARQUEO ".center(50))
    print("="*50)
    for tipo, zona in parqueo.zonas.items():
        print(f"- {zona['display_name']} ({tipo.capitalize()}): Filas {zona['start_row']+1} a {zona['end_row']+1}")
    print("="*50)

def mostrar_menu_admin(parqueo: Parqueadero) -> None:
    """Muestra el menú de opciones para el rol de administrador."""
    print("\n--- MENÚ ADMINISTRADOR ---")
    print("1. Estacionar vehículo")
    print("2. Retirar vehículo")
    print("3. Buscar vehículo")
    print("4. Generar reporte de ocupación")
    print("5. Generar reporte financiero")
    print("6. Hacer reserva")
    print("7. Ver reservas")
    print("8. Añadir evento especial (Tarifas)")
    print("0. Cerrar sesión")
    print("="*50)

def mostrar_menu_cliente(parqueo: Parqueadero) -> None:
    """Muestra el menú de opciones para el rol de cliente."""
    print("\n--- MENÚ CLIENTE ---")
    print("1. Estacionar vehículo")
    print("2. Retirar vehículo")
    print("3. Buscar vehículo")
    print("0. Cerrar sesión")
    print("="*50)

def main():
    """Función principal que inicia y gestiona el sistema de parqueadero."""
    db_manager = DatabaseManager()
    sistema_autenticacion = SistemaAutenticacion()
    historial = Historial()

    parqueo = ParqueaderoDB(10, 10, db_manager)
    parqueo.cargar_estado()

    tarifador = Tarifador()
    sistema_reservas = SistemaReservas(parqueo)
    report_generator = ReportGenerator()

    while True:
        usuario_actual = sistema_autenticacion.login()

        if not usuario_actual:
            break

        while True:
            parqueo.mostrar_mapa()
            mostrar_info_zonas(parqueo)
            tarifador.mostrar_tarifas_actuales()

            if usuario_actual.rol == 'admin':
                mostrar_menu_admin(parqueo)
                opcion = input("Seleccione una opción: ").strip()
                if opcion == '1':
                    parqueo.estacionar_vehiculo_interactivo(sistema_reservas)
                elif opcion == '2':
                    parqueo.retirar_vehiculo_interactivo(tarifador)
                elif opcion == '3':
                    identificador = input("Ingrese placa o ID del vehículo a buscar: ").strip().upper()
                    parqueo.buscar_vehiculo(identificador)
                elif opcion == '4':
                    report_generator.generar_reporte_ocupacion(parqueo)
                elif opcion == '5':
                    report_generator.generar_reporte_financiero()
                elif opcion == '6':
                    sistema_reservas.hacer_reserva(usuario_actual)
                elif opcion == '7':
                    sistema_reservas.ver_reservas()
                elif opcion == '8':
                    try:
                        fecha_str = input("Ingrese la fecha del evento (YYYY-MM-DD): ")
                        fecha_evento = datetime.datetime.strptime(fecha_str, "%Y-%m-%d").date()
                        desc_evento = input("Ingrese la descripción del evento: ")
                        tarifador.agregar_evento(fecha_evento, desc_evento, usuario_actual)
                        print("Evento especial añadido.")
                        log_evento(f"Admin {usuario_actual.username} añadió evento: {desc_evento} en {fecha_evento}.")
                    except ValueError:
                        print("Formato de fecha incorrecto.")
                        log_evento(f"Admin {usuario_actual.username} intentó añadir evento con fecha incorrecta.")
                elif opcion == '0':
                    print(f"Cerrando sesión de {usuario_actual.username}...")
                    log_evento(f"Sesión cerrada para {usuario_actual.username}")
                    break
                else:
                    print("Opción inválida. Intente de nuevo.")
                    log_evento(f"Admin {usuario_actual.username} ingresó opción inválida: '{opcion}'.")

            elif usuario_actual.rol == 'cliente':
                mostrar_menu_cliente(parqueo)
                opcion = input("Seleccione una opción: ").strip()
                if opcion == '1':
                    parqueo.estacionar_vehiculo_interactivo(sistema_reservas)
                elif opcion == '2':
                    parqueo.retirar_vehiculo_interactivo(tarifador)
                elif opcion == '3':
                    identificador = input("Ingrese placa o ID del vehículo a buscar: ").strip().upper()
                    parqueo.buscar_vehiculo(identificador)
                elif opcion == '0':
                    print(f"Cerrando sesión de {usuario_actual.username}...")
                    log_evento(f"Sesión cerrada para {usuario_actual.username}")
                    break
                else:
                    print("Opción inválida. Intente de nuevo.")
                    log_evento(f"Cliente {usuario_actual.username} ingresó opción inválida: '{opcion}'.")

    db_manager.close()
    print("Saliendo del sistema de parqueadero. ¡Hasta luego!")

if __name__ == "__main__":
    main()
