Hay que tener en cuenta que la mayorá de estas implementaciones se hicieron con el apoyo de IA.

**1. Modo historia:**
Este modo permite darle un poco mas de sentido a la historia, es importante puesto que le permite al jugador tener un sentido narrativo y tener un objetivo;en ese caso a Ash de pueblo paleta, quien hasta ahora va iniciar su viaje como entrenador y tendra que pasar por varios oponentes para ser mas fuerte y llegar a ser un Maestro Pokemon, para esto se tiene que pasar 10 nivles, en donde se podra ir teniendo un aequipo pokemon mas grande.En términos técnicos, esto implica:

- Una clase Historia o ModoHistoria:
  Aqui se tienen las funciones que manejan el desarrollo de la historia(textos, anvance y fin)

- Sistema de niveles (Nivel.h, Nivel.cpp, Entrenador.h...):
Para controlar como avanza la historia se necesita primero establecer una conexion con los otros h.files, declarando los objetos que se necesitan para que el modo funcione.

- Controlador del flujo (Historia::avanzar(), Historia::cargarNivel()):
Esto es para que la historia tenga un orden, y vaya guaindo al usuario, por medio de diálogos, decisiones, y eventos.

**2. Pokémon con logotipo:**
Para tener una mejor interfaz con el usurio se crearon cuadros, se le añadio colores a los textos y reacciones después de ciertas desiciones, y al principio teniendo en el menú el logo de pokemon. Esto se ve en la parte de utilidades.h y utilidades.cpp; con otras funciones para manejar esto como limpiar pantalla, el que no se ejecute una acción hasta que se oprima ENTER.
Para el logo se paso a lenguaje ASCII, en Utilidades.h se creo una función para q muestre el logo, y en utilidades.cpp se le dio la implementación a esta función poniendo el logo. Para los colores en estas mismos bloques de código mencionados anteriormente, aqui a cada pokemon se enuncian los colores ñara luego en el main y en otras partes donde se le agregue color al texto implementar esto. Por último para hacer simulacion de audios para una mejor experiencia visual, se creo un AudioManager. h y .cpp en donde en el primero se declaran todas funciones y en el segundo se le dan el texto con emojis que van a aparecer, esto se activa y se ubica la cabecera en donde se necesite utilizar.

**3. Batalla con personajes aleatorios:**
Tiene un modo de batalla en donde se entra se esoge a un pokemon incial y el sistema escoge un entrenador ramdom para que se haga un combate rapido, en cada combate se muestra el estado del pokemon después de cada ataque, cuando se usa el ataque muestra si este fue efectivo o no y cuantos puntos de daño recibio o le hizo al rival, volviendo a recargar la vida a los pokemon del combate. 
Para escoger un personaje y Pokémon aleatorio se usa los personajes ya creados en el modo historia, y con random se escoge uno, se usa el mismo sistema de batalla base que se uso para el modo historia.

**4. Que el entrenador pueda agrandar su equipo después de cada batalla:**
Después de cada batalla independientemente el tipo el entrenado puede escoger un pokemon para tener en su equipo y poder pelear con este en otras batallas. 
El Pokémon q se agrega es uno del rival, el usuario puede escoger cual Pokémon quiere, también cuando ya tenga mas de 6 pokemones en su equipo el sistema le dira q si quiere agregar uno tiene q abandonar uno. 
Para esto se usa crea un arreglo en donde se guarda el equipo Pokémon del jugador, si este oprime q si.
Para curar a los pokemones después de cada partida 


#Explicación de headers

Cada header tiene un propósito específico, contribuyendo a la modularidad y facilitando el desarrollo y la depuración del juego.

1. GlobalTypes.h
Propósito: Este archivo es crucial para definir tipos de datos globales, enumeraciones y constantes que son utilizados por múltiples clases en el proyecto. Centraliza estas definiciones, evitando duplicación y asegurando consistencia en todo el código. Si no tienes un GlobalTypes.h, estas definiciones podrían estar dispersas en otros headers, lo que hace el código menos organizado y más propenso a errores.

Funciones:

- enum class TipoPokemon: Define los diferentes tipos elementales de Pokémon (Agua, Fuego, Planta, Eléctrico, Normal, etc.). Es fundamental para la lógica de daño en combate (ventajas/desventajas de tipo).

- enum class StatusEffectType: Define los posibles estados alterados que un Pokémon puede sufrir (Veneno, Parálisis, Quemadura, Dormido, etc.). Crucial para la implementación de ataques con efectos secundarios y la lógica de curación.

- enum class ColorConsola: (Podría estar en Utilidades.h también, pero si son constantes globales, aquí). Define códigos para cambiar el color del texto en la consola, vital para la interfaz de usuario.

Relación con Otros Headers:

Incluido por: Pokemon.h, Ataque.h, Utilidades.h (si define colores), y cualquier otro archivo que necesite conocer los tipos de Pokémon o efectos de estado.

2. Ataque.h
Propósito/Importancia: Este archivo define la estructura o clase que representa un ataque en el juego. Es fundamental para la mecánica de combate, ya que cada Pokémon tendrá una lista de ataques que puede usar. Separar la definición de Ataque permite que Pokemon solo se enfoque en sus características y no en los detalles de cómo es un ataque.

Funciones/Clases Base (y su importancia):

class Ataque:

Miembros: nombre, tipo (TipoPokemon), poder (daño base), precision (probabilidad de acertar), descripcion, efectoSecundario (opcional, para estados alterados).

Importancia: Es la base para todos los movimientos que los Pokémon pueden aprender y usar. Su estructura permite modelar la complejidad de los ataques del juego.

Relación con Otros Headers:

Incluye: GlobalTypes.h (para TipoPokemon, StatusEffectType).

Incluido por: Pokemon.h (para que los Pokémon puedan tener ataques).

3. Pokemon.h
Propósito/Importancia: Este es uno de los pilares del juego. Define la clase Pokemon, que encapsula todas las características y comportamientos de una criatura Pokémon. Su objetivo es modelar un Pokémon individual y sus capacidades en combate.

Funciones/Clases Base (y su importancia):

class Pokemon:

Miembros: nombre, tipo (TipoPokemon), vidaActual, vidaMaxima, ataque, defensa, velocidad, listaAtaques (vector de Ataque), estado (StatusEffectType).

Importancia: Representa a cada criatura en el juego, tanto las del jugador como las del oponente. Contiene sus estadísticas, tipo, y los ataques que puede realizar.

Métodos:

usarAtaque(const Ataque& ataque, Pokemon& objetivo): La lógica principal de cómo un Pokémon ataca a otro. Calcula el daño, aplica efectos de estado, etc.

recibirDanio(int danio): Cómo un Pokémon reacciona al ser golpeado, reduciendo su vida.

estaConsciente() const: Verifica si el Pokémon aún puede luchar (vida > 0).

mostrarInfo() const: Muestra las estadísticas y el estado del Pokémon.

mostrarAtaques() const: Lista los ataques disponibles del Pokémon.

getNumeroAtaques() const, getAtaque(int index) const: Métodos para acceder a los ataques de forma segura.

Relación con Otros Headers:

Incluye: Ataque.h, GlobalTypes.h.

Incluido por: Entrenador.h, Nivel.h, main.cpp (para crear instancias de Pokémon).

4. Entrenador.h
Propósito/Importancia: Define la clase Entrenador, que representa a cualquier personaje que posea Pokémon, incluyendo al jugador y a los oponentes (líderes de gimnasio, entrenadores salvajes, etc.). Su rol es gestionar el equipo de Pokémon y las posesiones del entrenador.

Funciones/Clases Base (y su importancia):

class Entrenador:

Miembros: nombre, equipoPokemon (vector de Pokemon), pokemonActivoIndex (índice del Pokémon en combate), medallas (vector de std::string para las medallas).

Importancia: Encapsula el estado del jugador y de los líderes de gimnasio, permitiendo gestionar sus equipos y su progresión (medallas).

Métodos:

agregarPokemon(const Pokemon& pokemon): Añade un Pokémon al equipo.

mostrarEquipo() const: Muestra los Pokémon del equipo del entrenador.

getPokemonActivo(): Obtiene el Pokémon que está luchando actualmente.

tienePokemonesConscientes() const: Verifica si el entrenador tiene Pokémon disponibles para luchar.

cambiarPokemonActivo(int index): Permite al entrenador cambiar su Pokémon en combate.

curarPokemones(): Cura a todos los Pokémon del equipo (como en un Centro Pokémon).

agregarMedalla(const std::string& medalla), tieneMedalla(const std::string& medalla) const, getNumeroMedallas() const: Métodos para gestionar las medallas del jugador.

Relación con Otros Headers:

Incluye: Pokemon.h.

Incluido por: Nivel.h, Historia.h, main.cpp.

5. Nivel.h
Propósito/Importancia: Define la clase Nivel, que orquesta los combates entre dos entrenadores (jugador y oponente). Su responsabilidad es manejar la lógica de un solo combate, incluyendo turnos, uso de ataques, cambios de Pokémon, y determinar el ganador.

Funciones/Clases Base (y su importancia):

class Nivel:

Miembros: jugador (referencia a Entrenador), oponente (referencia a Entrenador), nivelActual (para escalar la dificultad del oponente).

Importancia: Es el corazón de la interacción de combate en el juego. Cada vez que el jugador lucha contra un entrenador o un líder de gimnasio, se crea una instancia de Nivel.

Métodos:

configurarEquipoOponente(): Prepara el equipo del oponente para la batalla, escalando su dificultad.

iniciarCombate(): El método principal que ejecuta el ciclo de combate hasta que un entrenador pierde todos sus Pokémon.

turnoJugador(): Maneja las acciones que el jugador puede realizar en su turno (atacar, cambiar Pokémon).

turnoOponente(): Implementa la lógica de IA simple para el turno del oponente.

Relación con Otros Headers:

Incluye: Entrenador.h, Pokemon.h (indirectamente a través de Entrenador.h), Utilidades.h, AudioManager.h.

Incluido por: Historia.h, main.cpp.

6. Historia.h
Propósito/Importancia: La clase Historia es el "director" del juego, controlando la progresión narrativa, la secuencia de gimnasios y otros eventos importantes. Su objetivo es guiar al jugador a través de la aventura.

Funciones/Clases Base (y su importancia):

class Historia:

Miembros: jugador (referencia a Entrenador), oponentesDisponibles (vector de Entrenador para los líderes de gimnasio/rivales), nivelActual (el gimnasio actual), historiaCompleta (bandera de estado).

Importancia: Conecta los diferentes combates (Nivel) en una progresión lógica, otorgando medallas y llevando al jugador a través de la narrativa del juego.

Métodos:

iniciarAventura(): El punto de entrada para el modo historia.

iniciarNiveles(): Gestiona la secuencia de los combates de gimnasio, la curación y la lógica de victoria/derrota para cada gimnasio.

esHistoriaCompleta() const: Permite al main saber si el jugador ha terminado la aventura.

introduccion(), capituloCiudadInicial(), mostrarNarrativa(), obtenerEleccion(): Métodos auxiliares para la narrativa y la interacción con el jugador.

Relación con Otros Headers:

Incluye: Entrenador.h, Nivel.h, Utilidades.h, AudioManager.h.

Incluido por: main.cpp.

7. Utilidades.h
Propósito/Importancia: Este archivo agrupa funciones y constantes de utilidad general que no pertenecen lógicamente a ninguna otra clase específica del juego. Su objetivo principal es encapsular operaciones comunes como limpiar la pantalla, pausar el juego, imprimir texto con colores, dibujar elementos ASCII, etc., promoviendo la reutilización de código y manteniendo los otros archivos más limpios.

Funciones/Clases Base (y su importancia):

namespace Utilidades: Se usa un namespace para agrupar estas funciones y evitar colisiones de nombres.

Funciones estáticas/globales:

limpiarPantalla(): Borra el contenido de la consola.

pausar(): Detiene la ejecución hasta que el usuario presione una tecla.

imprimirEnColor(std::string texto, ColorConsola color, ...): Imprime texto en la consola con colores definidos (requiere Windows.h para sistemas Windows o códigos ANSI para otros).

dibujarSeparador(char caracter, int longitud, ColorConsola color): Dibuja una línea de separación en la consola.

dibujarRecuadro(std::string titulo, std::string mensaje, ColorConsola color, bool centrar): Crea un cuadro de texto para mensajes importantes.

limpiarBufferEntrada(): Soluciona problemas con std::cin y las nuevas líneas.

mostrarPokeballASCII(), mostrarCentroPokemonASCII(), mostrarEfectoCuracionASCII(), etc.: Funciones que imprimen arte ASCII específico para mejorar la experiencia visual.

Relación con Otros Headers:

Incluye: GlobalTypes.h (si ColorConsola está ahí), <iostream>, <string>, <thread>, <chrono>, y potencialmente <windows.h>.

Incluido por: Pokemon.h, Entrenador.h, Nivel.h, Historia.h, main.cpp (prácticamente todos los archivos que interactúan con la consola).

8. AudioManager.h (Asumiendo que lo tienes o lo quieres implementar)
Propósito/Importancia: Si tu juego va a tener sonido (música y efectos sonoros), esta clase es fundamental. Su propósito es gestionar la reproducción de audio, centralizando la lógica de sonido para que otras partes del juego simplemente "pidan" un sonido sin preocuparse por cómo se reproduce. Utiliza el patrón Singleton para asegurar una única instancia del gestor de audio.

Funciones/Clases Base (y su importancia):

class AudioManager (Singleton):

Miembros: Punteros a objetos de librerías de audio (ej. Mix_Music*, Mix_Chunk* si usas SDL_mixer), mapas para almacenar sonidos por nombre.

Importancia: Permite añadir música de fondo, efectos de ataque, sonidos de victoria/derrota, etc., mejorando drásticamente la inmersión del jugador.

Métodos:

getInstance(): El método estático para obtener la única instancia del AudioManager.

cargarMusica(const std::string& nombre, const std::string& ruta): Carga un archivo de música.

reproducirMusicaFondo(const std::string& nombre, int loops = -1): Reproduce música en bucle.

detenerMusica(): Detiene la música actual.

cargarEfectoSonoro(const std::string& nombre, const std::string& ruta): Carga un efecto de sonido.

reproducirEfectoSonoro(const std::string& nombre): Reproduce un efecto de sonido una vez.

liberarRecursos(): Libera la memoria ocupada por los sonidos cargados.

Relación con Otros Headers:

Incluye: Librerías de audio externas (ej. <SDL2/SDL_mixer.h>), <string>, <map>.

Incluido por: Nivel.h, Historia.h, main.cpp (cualquier lugar donde se necesite sonido).

Cada uno de estos headers trabaja en conjunto para construir tu juego, siguiendo principios de programación modular y orientada a objetos. Una buena organización de headers es clave para la escalabilidad y el mantenimiento de tu proyecto.
