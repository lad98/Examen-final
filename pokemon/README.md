**1. Modo historia:**
Este modo permite darle un poco mas de sentido a la historia en ese caso a Ash de pueblo paleta, quien hasta ahora va iniciar su viaje como entrenador y tendra que pasar por varios oponentes para ser mas fuerte y llegar a ser un Maestro Pokemon, para esto se tiene que pasar 10 nivles, en donde se podra ir teniendo un aequipo pokemon mas grande.En términos técnicos, esto implica:

- Una clase Historia o ModoHistoria:
  Aqui se tienen las funciones que manejan el desarrollo de la historia(textos, anvance y fin)

- Sistema de niveles (Nivel.h, Nivel.cpp, Entrenador.h...):
Para controlar como avanza la historia se necesita primero establecer una conexion con los otros h.files, declarando los objetos que se necesitan para que el modo funcione.

- Controlador del flujo (Historia::avanzar(), Historia::cargarNivel()):
Esto es para que la historia tenga un orden, y vaya guaindo al usuario, por medio de diálogos, decisiones, y eventos.

**2. Pokémon con logotipo:**
Para tener una mejor interfaz con el usurio se crearon cuadros, se le añadio colores a los textos y reacciones después de ciertas desiciones, y al principio teniendo en el menú el logo de pokemon. Esto se ve en la parte de utilidades.h y utilidades.cpp; con otras funciones para manejar esto como limpiar pantalla, el que no se ejecute una acción hasta que se oprima ENTER.

**3. Batalla con personajes aleatorios:**
Tiene un modo de batalla en donde se entra se esoge a un pokemon incial y el sistema escoge un entrenador ramdom para que se haga un combate rapido, en cada combate se muestra el estado del pokemon después de cada ataque, cuando se usa el ataque muestra si este fue efectivo o no y cuantos puntos de daño recibio o le hizo al rival, volviendo a recargar la vida a los pokemon del combate.

**4. Que el entrenador pueda agrandar su equipo después de cada batalla:**
Después de cada batalla independientemente el tipo el entrenado puede escoger un pokemon para tener en su equipo y poder pelear con este en otras batallas.
