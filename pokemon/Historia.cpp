#include "Historia.h"
#include <iostream> 
#include <string>   
Historia::Historia(Entrenador& jugadorRef, std::vector<Entrenador>& oponentesRef)
    : jugador(jugadorRef), oponentes(oponentesRef), historiaCompleta(false) {} 


void Historia::mostrarNarrativa(const std::string& texto) {
    Utilidades::dibujarSeparador('*', 60, Utilidades::AMARILLO);
    Utilidades::imprimirEnColor(texto, Utilidades::BLANCO, false);
    Utilidades::dibujarSeparador('*', 60, Utilidades::AMARILLO);
    Utilidades::pausar();
}


int Historia::obtenerEleccion(const std::vector<std::string>& opciones) {
    int eleccion;
    bool valida = false;
    do {
        for (size_t i = 0; i < opciones.size(); ++i) {
            std::cout << (i + 1) << ". " << opciones[i] << std::endl;
        }
        std::cout << Utilidades::AMARILLO << "Tu elección: " << Utilidades::RESET;
        if (std::cin >> eleccion && eleccion > 0 && eleccion <= opciones.size()) {
            valida = true;
        } else {
            Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número válido.", Utilidades::ROJO, false);
            std::cin.clear();
            Utilidades::limpiarBufferEntrada();
        }
    } while (!valida);
    Utilidades::limpiarBufferEntrada(); 
    return eleccion;
}


void Historia::introduccion() {
    Utilidades::limpiarPantalla();
    mostrarNarrativa("¡Bienvenido al mundo de Kanto! Eres un joven entrenador que acaba de cumplir 10 años.");
    mostrarNarrativa("Es el día que has esperado toda tu vida: ¡el día en que recibirás tu primer Pokémon!");
    mostrarNarrativa("Tu aventura está a punto de comenzar...");
}


void Historia::capituloCiudadInicial() {
    Utilidades::limpiarPantalla();
    mostrarNarrativa("Llegas al laboratorio del Profesor Oak. Él te espera con una sonrisa.");
    mostrarNarrativa("Profesor Oak: ¡Hola, " + jugador.nombre + "! Es un placer verte. ¿Estás listo para tu Pokémon?");

   
    mostrarNarrativa("Después de elegir a " + jugador.getPokemonActivo().nombre + ", el Profesor Oak te da una PokéDex y algunas Poké Balls.");
    mostrarNarrativa("Profesor Oak: ¡Ahora ve y explora el mundo! ¡Conviértete en un Maestro Pokémon!");
}


void Historia::iniciarNiveles() {
    capituloCiudadInicial(); 

   
    for (size_t i = 0; i < oponentes.size(); ++i) {
        int intentosRestantes = 2;
        bool nivelSuperado = false;

        while (intentosRestantes > 0 && !nivelSuperado) {
            Utilidades::limpiarPantalla();
            mostrarNarrativa("¡Prepárate para el Nivel " + std::to_string(i + 1) + "!");
            mostrarNarrativa("Tu oponente es " + oponentes[i].nombre + ".");
            if (intentosRestantes == 2) {
                mostrarNarrativa("Tienes 2 oportunidades para superar este nivel.");
            } else {
                mostrarNarrativa("Te queda 1 oportunidad para superar este nivel. ¡No te rindas!");
            }

            Nivel nivel(jugador, oponentes[i], i + 1); 
            bool victoria = nivel.iniciarCombate(); 

            if (victoria) {
                mostrarNarrativa("¡Felicidades! Has derrotado a " + oponentes[i].nombre + " en el Nivel " + std::to_string(i + 1) + ".");
                nivelSuperado = true;
                jugador.curarPokemones(); 
                Utilidades::pausar();
            } else {
                intentosRestantes--;
                if (intentosRestantes > 0 && jugador.tienePokemonesConscientes()) { 
                    mostrarNarrativa("Has sido derrotado en el Nivel " + std::to_string(i + 1) + ". ¡Pero no te preocupes, tienes otra oportunidad!");
                    jugador.curarPokemones(); 
                    Utilidades::pausar();
                } else {
                    mostrarNarrativa("Has agotado tus oportunidades para el Nivel " + std::to_string(i + 1) + ".");
                    mostrarNarrativa("Tu aventura ha terminado aquí. ¡Mejor suerte la próxima vez!");
                    Utilidades::pausar();
                    historiaCompleta = false; 
                    return; 
                }
            }
        }
      
        if (!jugador.tienePokemonesConscientes()) {
            historiaCompleta = false;
            return;
        }
    }

    
    Utilidades::limpiarPantalla();
    mostrarNarrativa("¡Felicidades! ¡Has derrotado a todos los entrenadores y te has convertido en un maestro Pokémon!");
    mostrarNarrativa("Tu viaje ha sido largo y lleno de desafíos, pero tu determinación te ha llevado a la cima.");
    mostrarNarrativa("¡El mundo Pokémon siempre tendrá nuevas aventuras para ti!");
    historiaCompleta = true; 
}


void Historia::iniciarAventura() {
    introduccion();
    iniciarNiveles(); 
}


bool Historia::esHistoriaCompleta() const {
    return historiaCompleta;
}
