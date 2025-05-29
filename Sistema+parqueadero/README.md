Todas estas implementaciones usaron IA para poder realizarse.

Clase Config

 * Propósito: Almacena constantes de configuración del sistema.
 * Importancia: Centraliza todos los valores que pueden cambiar (nombres de archivos, períodos de gracia, tipos de vehículos, tarifas de lavado, nombre de la base de datos).
 * ¿Por qué es importante en el código?   Permite modificar fácilmente estos valores sin tener que buscar y cambiar cada instancia en el código. Mejora la legibilidad y mantenibilidad.
   
Clase TipoVehiculo

 * Propósito: Define los tipos de vehículos permitidos en el parqueadero como una enumeración.
 * Importancia: Proporciona un conjunto fijo y seguro de tipos de vehículos, evitando errores de escritura y haciendo el código más legible.
 * ¿Por qué es importante en el código?    Garantiza la consistencia de los datos y facilita la validación de entradas, ya que solo se aceptan los tipos predefinidos.
   
Funciones de Utilidad Globales

 * log_evento(mensaje: str) -> None
   * Propósito: Registra eventos importantes del sistema en un archivo de texto (parqueadero_log.txt) con una marca de tiempo.
   * Importancia: Es fundamental para la auditoría, el seguimiento de operaciones, la depuración de errores y la generación de reportes históricos.
   * ¿Por qué es importante en el código?    Permite saber qué ocurrió, cuándo y por qué, lo cual es invaluable para el diagnóstico y la operación del sistema.
 * mostrar_confirmacion(pregunta: str) -> bool
   * Propósito: Muestra una pregunta al usuario y espera una respuesta 's' (sí) o 'n' (no).
   * Importancia: Mejora la interacción con el usuario, permitiendo confirmaciones antes de realizar acciones críticas.
   * ¿Por qué es importante en el código?   Previene errores por acciones accidentales del usuario y guía al usuario a través de decisiones.
 * mostrar_progreso(mensaje: str, duracion: int = 2) -> None
   * Propósito: Simula una barra de progreso en la consola.
   * Importancia: Ofrece retroalimentación visual al usuario durante operaciones que pueden tomar un tiempo, mejorando la experiencia de usuario.
   * ¿Por qué es importante en el código?    Aunque no es crítica para la lógica central, hace que la aplicación se sienta más responsiva y amigable.
     
Clase Usuario

 * Propósito: Representa a un usuario del sistema, almacenando su nombre de usuario, contraseña (encriptada) y rol (admin o cliente).
 * Importancia: Es la base para la autenticación y la gestión de permisos, permitiendo diferenciar las funcionalidades disponibles para cada tipo de usuario.
 * ¿Por qué es importante en el código?
   * __init__(self, username: str, password: str, rol: str): Inicializa un objeto Usuario. Es crucial para crear usuarios con sus atributos básicos, incluyendo la encriptación inicial de la contraseña.
   * _encriptar_password(self, password: str) -> str: Encripta la contraseña usando SHA256. Es una medida de seguridad fundamental.
   * verificar_password(self, password: str) -> bool: Compara una contraseña ingresada con la encriptada almacenada. Es la clave para la autenticación.
   * actualizar_ultimo_acceso(self): Registra la última vez que el usuario accedió. Útil para auditoría y posibles futuras funcionalidades de sesión.
     
Clase SistemaAutenticacion

 * Propósito: Gestiona el proceso de inicio de sesión de los usuarios.
 * Importancia: Es la puerta de entrada al sistema, asegurando que solo los usuarios autorizados puedan acceder y que se les asignen las funcionalidades adecuadas según su rol.
 * ¿Por qué es importante en el código?
   * __init__(self): Inicializa el sistema con un conjunto predefinido de usuarios (admin y cliente). Es vital para tener usuarios listos para probar el sistema.
   * login(self) -> Optional[Usuario]: Maneja la interacción de inicio de sesión, pidiendo usuario y contraseña, y validándolos. Es la función principal para controlar el acceso.
     
Clase DatabaseManager

 * Propósito: Abstrae las operaciones de interacción con la base de datos SQLite.
 * Importancia: Centraliza la lógica de persistencia de datos, desacoplando el resto del sistema de los detalles de la base de datos.
 * ¿Por qué es importante en el código?
   * __init__(self, db_name: str = Config.DB_NAME): Establece la conexión inicial a la base de datos. Es el punto de partida para cualquier operación de DB.
   * _connect(self): Maneja la conexión real a la base de datos. Es crucial para asegurar que la aplicación pueda leer y escribir datos.
   * _create_tables(self): Crea la estructura de la tabla vehiculos si no existe. Es vital para la integridad de los datos y para que la aplicación funcione la primera vez.
   * guardar_vehiculo(...): Inserta o actualiza un registro de vehículo. Es fundamental para la persistencia del estado de los vehículos.
   * eliminar_vehiculo(...): Elimina un registro de vehículo. Necesario para cuando un vehículo se retira.
   * cargar_vehiculos(...): Carga todos los vehículos estacionados desde la DB. Es esencial para restaurar el estado del parqueadero al iniciar la aplicación.
   * close(self): Cierra la conexión a la base de datos. Importante para liberar recursos y evitar corrupción de la base de datos.
     
Clase Historial

 * Propósito: Proporciona una interfaz para registrar eventos en el log del sistema.
 * Importancia: Permite un seguimiento detallado de las operaciones del parqueadero, lo cual es útil para auditorías, análisis de rendimiento y depuración.
 * ¿Por qué es importante en el código?
   * __init__(self): Inicializa el módulo de historial.
   * registrar_evento(...): Es el método que se llama desde otras partes del código para añadir entradas al log.
     
Clase ReportGenerator

 * Propósito: Genera reportes de ocupación y financieros basados en los datos del parqueadero y el log de eventos.
 * Importancia: Proporciona información clave para la gestión del parqueadero, ayudando a los administradores a entender el uso y el rendimiento financiero.
 * ¿Por qué es importante en el código?
   * generar_reporte_ocupacion(...): Calcula y muestra estadísticas sobre cuántos espacios están ocupados, libres y la distribución por zonas. Es vital para la gestión de capacidad.
   * generar_reporte_financiero(...): Procesa el archivo de log para sumar ingresos y servicios. Es crucial para el seguimiento de la rentabilidad.
     
Clase SistemaReservas

 * Propósito: Gestiona la funcionalidad de reserva de espacios de parqueo.
 * Importancia: Permite a los usuarios (especialmente administradores) pre-asignar espacios, lo que puede mejorar la eficiencia y la satisfacción del cliente.
 * ¿Por qué es importante en el código?
   * __init__(self, parqueadero: Any): Inicializa el sistema de reservas, vinculándolo al objeto Parqueadero para interactuar con el mapa.
   * hacer_reserva(self, usuario: Usuario): Permite a un usuario (con rol de administrador) crear una reserva, validando la ubicación y el tipo de vehículo. Es el método central para la creación de reservas.
   * ver_reservas(self): Muestra una lista de todas las reservas activas. Es importante para la visibilidad de las reservas.
   * verificar_y_aplicar_reserva(...): Se llama cuando un vehículo intenta estacionar para ver si hay una reserva que coincida y, si la hay, la aplica y la elimina. Es crucial para la integración de reservas con el estacionamiento real.
     
Clase Tarifador

 * Propósito: Calcula el costo del estacionamiento, aplicando tarifas base y factores dinámicos (hora pico, fin de semana, eventos especiales).
 * Importancia: Es el motor financiero del sistema, asegurando que los costos se calculen de manera justa y consistente.
 * ¿Por qué es importante en el código?
   * __init__(self): Define las tarifas base y los factores dinámicos. Es la configuración inicial del modelo de precios.
   * agregar_evento(...): Permite a los administradores introducir eventos especiales que afectan las tarifas. Es vital para la flexibilidad de precios.
   * calcular_tarifa(...): La función principal que toma el tipo de vehículo y el tiempo estacionado para determinar el costo final, aplicando todos los factores. Es el corazón del sistema de facturación.
   * mostrar_tarifas_actuales(self): Informa a los usuarios sobre las tarifas actuales, incluyendo los factores dinámicos aplicables. Es importante para la transparencia.
     
Clase Factura

 * Propósito: Representa una factura detallada por el servicio de parqueo y lavado.
 * Importancia: Proporciona un registro claro y comprensible de los costos para el cliente y para los registros financieros del parqueadero.
 * ¿Por qué es importante en el código?
   * __init__(...): Constructor que recopila todos los detalles necesarios para una factura.
   * _generar_id_factura(self): Crea un identificador único para cada factura. Es esencial para el seguimiento de transacciones.
   * mostrar_factura(self): Imprime una versión legible de la factura en la consola. Es la forma en que el cliente ve su desglose de costos.
     
