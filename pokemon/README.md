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
