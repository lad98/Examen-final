#include <iostream> 
#include <vector>   
#include <string>   
#include <random>   
#include <ctime>    
#include "Pokemon.h"
#include "Ataque.h"
#include "Entrenador.h"
#include "Nivel.h"
#include "Utilidades.h" 
#include "GlobalTypes.h" 
#include "Historia.h" 
#include "AudioManager.h" 

Pokemon crearPokemon(std::string nombre, std::string tipo, int vida, int ataque, int defensa,
                     const std::vector<Ataque>& ataquesDisponibles, const std::vector<int>& indicesAtaques) {
    Pokemon p(nombre, tipo, vida, ataque, defensa); 
    for (int indice : indicesAtaques) { 
        if (indice >= 0 && indice < ataquesDisponibles.size()) {
            p.agregarAtaque(ataquesDisponibles[indice]);
        }
    }
    return p; 
}

void elegirPokemonInicial(Entrenador& jugador, const std::vector<Ataque>& ataquesBase) {
    Pokemon pikachu = crearPokemon("Pikachu", "Electrico", 90, 65, 45, ataquesBase, {5, 8, 0, 15});
    Pokemon charmander = crearPokemon("Charmander", "Fuego", 80, 70, 40, ataquesBase, {2, 9, 0, 16});
    Pokemon squirtle = crearPokemon("Squirtle", "Agua", 95, 60, 50, ataquesBase, {3, 10, 7, 17});
    Pokemon bulbasaur = crearPokemon("Bulbasaur", "Planta", 100, 55, 45, ataquesBase, {4, 11, 6, 18});

    int eleccionInicial;
    bool eleccionValida = false;

    do {
        Utilidades::limpiarPantalla();
        Utilidades::dibujarRecuadro("ELIGE TU POKÉMON INICIAL", "¡Tu compañero de aventura te espera!", Utilidades::AMARILLO, true);
        std::cout << Utilidades::CYAN << "1. Pikachu" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::ROJO << "2. Charmander" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::AZUL << "3. Squirtle" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::VERDE << "4. Bulbasaur" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::AMARILLO << "Tu elección: " << Utilidades::RESET;

        if (!(std::cin >> eleccionInicial)) {
            std::cin.clear();
            Utilidades::limpiarBufferEntrada();
            Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
            Utilidades::pausar();
            eleccionValida = false;
        } else {
            Utilidades::limpiarBufferEntrada();
            switch (eleccionInicial) {
                case 1: jugador.agregarPokemon(pikachu); eleccionValida = true; break;
                case 2: jugador.agregarPokemon(charmander); eleccionValida = true; break;
                case 3: jugador.agregarPokemon(squirtle); eleccionValida = true; break;
                case 4: jugador.agregarPokemon(bulbasaur); eleccionValida = true; break;
                default:
                    Utilidades::imprimirEnColor("¡Opción inválida! Intenta de nuevo.", Utilidades::ROJO, false);
                    Utilidades::pausar();
                    eleccionValida = false;
                    break;
            }
        }
    } while (!eleccionValida);

    Utilidades::imprimirEnColor("\n¡Has elegido a " + jugador.getPokemonActivo().nombre + "!", Utilidades::VERDE, true);
    AudioManager::getInstance()->reproducirEfectoSonoro("pokemon_elegido"); 
    Utilidades::pausar();
}



void iniciarModoHistoria(const std::vector<Ataque>& ataquesBase, std::vector<Entrenador>& oponentesDisponibles) {
    Entrenador jugador("Ash"); 
    elegirPokemonInicial(jugador, ataquesBase);

    Historia aventura(jugador, oponentesDisponibles);
    aventura.iniciarAventura(); 
    
    if (!jugador.tienePokemonesConscientes()) {
        Utilidades::imprimirEnColor("\n¡Has perdido el juego! Mejor suerte la próxima vez.", Utilidades::ROJO, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("derrota"); 
    } else if (aventura.esHistoriaCompleta()) { 
        Utilidades::imprimirEnColor("\n¡Felicidades! ¡Has completado la historia Pokémon!", Utilidades::VERDE, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("victoria"); 
    }
    Utilidades::pausar(); 
}


void iniciarModoBatalla(const std::vector<Ataque>& ataquesBase, const std::vector<Entrenador>& oponentesDisponibles) {
    Entrenador jugador("Jugador"); 
    elegirPokemonInicial(jugador, ataquesBase);

    Utilidades::limpiarPantalla();
    Utilidades::dibujarRecuadro("MODO BATALLA", "¡Prepárate para un combate!", Utilidades::CYAN, true);
    Utilidades::pausar();

    
    std::mt19937 generador(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<> dist(0, oponentesDisponibles.size() - 1);
    int indiceOponente = dist(generador);

    
    Entrenador oponenteSeleccionado = oponentesDisponibles[indiceOponente];

    Utilidades::imprimirEnColor("Te enfrentarás a " + oponenteSeleccionado.nombre + "!", Utilidades::AMARILLO, true);
    Utilidades::pausar();

    Nivel batallaUnica(jugador, oponenteSeleccionado, 0); 
    bool victoria = batallaUnica.iniciarCombate();

    if (victoria) {
        Utilidades::imprimirEnColor("\n¡Has ganado la batalla!", Utilidades::VERDE, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("victoria"); 
    } else {
        Utilidades::imprimirEnColor("\n¡Has perdido la batalla! Intenta de nuevo.", Utilidades::ROJO, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("derrota"); 
    }
    Utilidades::pausar();
}


}
int main() {
    
    srand(static_cast<unsigned int>(time(nullptr)));

    
    AudioManager* audio = AudioManager::getInstance();
    audio->toggleAudio(true); 
    
    std::vector<Ataque> ataquesBase = {
        Ataque("Placaje", 40, "Normal"),
        Ataque("Arañazo", 40, "Normal"),
        Ataque("Ascuas", 40, "Fuego"),
        Ataque("Pistola Agua", 40, "Agua"),
        Ataque("Hoja Afilada", 55, "Planta"),
        Ataque("Impactrueno", 40, "Electrico"),
        Ataque("Latigazo", 45, "Planta"),
        Ataque("Burbuja", 20, "Agua"),
        Ataque("Rayo", 60, "Electrico"),
        Ataque("Lanzallamas", 60, "Fuego"),
        Ataque("Hidrobomba", 60, "Agua"),
        Ataque("Rayo Solar", 60, "Planta"),
        Ataque("Picotazo Venenoso", 15, "Veneno", StatusEffectType::Poisoned, 30),
        Ataque("Bomba Lodo", 65, "Veneno", StatusEffectType::Poisoned, 10),
        Ataque("Golpe Roca", 40, "Roca"),
        Ataque("Excavar", 60, "Tierra"),
        Ataque("Mordisco", 60, "Siniestro"),
        Ataque("Psicorrayo", 65, "Psiquico"),
        Ataque("Puño Dinámico", 70, "Lucha")
    };

    
    std::vector<Entrenador> oponentes;

    
    Entrenador brock("Brock");
    brock.agregarPokemon(crearPokemon("Geodude", "Roca", 70, 45, 60, ataquesBase, {0, 14, 15}));
    brock.agregarPokemon(crearPokemon("Onix", "Roca", 120, 50, 90, ataquesBase, {0, 14, 15}));
    oponentes.push_back(brock);

    
    Entrenador misty("Misty");
    misty.agregarPokemon(crearPokemon("Staryu", "Agua", 80, 50, 40, ataquesBase, {3, 7, 10}));
    misty.agregarPokemon(crearPokemon("Starmie", "Agua", 100, 65, 55, ataquesBase, {3, 10, 17}));
    oponentes.push_back(misty);

    
    Entrenador ltSurge("Lt. Surge");
    ltSurge.agregarPokemon(crearPokemon("Voltorb", "Electrico", 75, 45, 35, ataquesBase, {5, 8}));
    ltSurge.agregarPokemon(crearPokemon("Raichu", "Electrico", 105, 75, 50, ataquesBase, {5, 8, 0, 16}));
    oponentes.push_back(ltSurge);

    
    Entrenador erika("Erika");
    erika.agregarPokemon(crearPokemon("Bellsprout", "Planta", 70, 45, 30, ataquesBase, {4, 12, 6}));
    erika.agregarPokemon(crearPokemon("Tangela", "Planta", 90, 55, 45, ataquesBase, {4, 11, 12}));
    oponentes.push_back(erika);

    
    Entrenador giovanni("Giovanni");
    giovanni.agregarPokemon(crearPokemon("Rhyhorn", "Tierra", 100, 70, 60, ataquesBase, {0, 14, 15}));
    giovanni.agregarPokemon(crearPokemon("Nidoqueen", "Tierra", 120, 75, 65, ataquesBase, {13, 0, 16}));
    giovanni.agregarPokemon(crearPokemon("Rhydon", "Tierra", 130, 85, 75, ataquesBase, {0, 14, 15}));
    oponentes.push_back(giovanni);

    
    Entrenador blaine("Blaine");
    blaine.agregarPokemon(crearPokemon("Growlithe", "Fuego", 110, 80, 60, ataquesBase, {2, 9, 0, 16}));
    blaine.agregarPokemon(crearPokemon("Arcanine", "Fuego", 130, 90, 70, ataquesBase, {2, 9, 0, 16}));
    oponentes.push_back(blaine);

   
    Entrenador sabrina("Sabrina");
    sabrina.agregarPokemon(crearPokemon("Kadabra", "Psiquico", 95, 70, 50, ataquesBase, {17, 18, 0, 1}));
    sabrina.agregarPokemon(crearPokemon("Alakazam", "Psiquico", 110, 85, 60, ataquesBase, {17, 18, 0, 1}));
    oponentes.push_back(sabrina);

    
    Entrenador koga("Koga");
    koga.agregarPokemon(crearPokemon("Grimer", "Veneno", 100, 70, 55, ataquesBase, {12, 13, 0, 1}));
    koga.agregarPokemon(crearPokemon("Muk", "Veneno", 120, 80, 65, ataquesBase, {12, 13, 0, 1}));
    oponentes.push_back(koga);

  
    Entrenador bruno("Bruno");
    bruno.agregarPokemon(crearPokemon("Machoke", "Lucha", 115, 85, 70, ataquesBase, {18, 14, 0, 1}));
    bruno.agregarPokemon(crearPokemon("Machamp", "Lucha", 135, 95, 80, ataquesBase, {18, 14, 0, 1}));
    oponentes.push_back(bruno);

   
    Entrenador lance("Lance");
    lance.agregarPokemon(crearPokemon("Dragonair", "Dragon", 125, 90, 75, ataquesBase, {0, 1, 15, 16}));
    lance.agregarPokemon(crearPokemon("Dragonite", "Dragon", 140, 100, 85, ataquesBase, {0, 1, 15, 16}));
    oponentes.push_back(lance);

    
    Utilidades::mostrarLogoPokemonASCII();
    audio->reproducirEfectoSonoro("inicio_juego"); 

    int opcion;
    do {
        Utilidades::limpiarPantalla();
        Utilidades::mostrarLogoPokemonASCII(); 
        Utilidades::imprimirEnColor("\n--- MENÚ PRINCIPAL ---", Utilidades::AZUL, true);
        std::cout << Utilidades::VERDE << "1. Iniciar Modo Historia" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::CYAN << "2. Iniciar Modo Batalla" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::ROJO << "3. Salir" << Utilidades::RESET << std::endl;
        std::cout << Utilidades::AMARILLO << "Elige una opción: " << Utilidades::RESET;

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            Utilidades::limpiarBufferEntrada();
            Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
            Utilidades::pausar();
            opcion = 0;
        } else {
            Utilidades::limpiarBufferEntrada();
            switch (opcion) {
                case 1:
                    iniciarModoHistoria(ataquesBase, oponentes);
                    break;
                case 2:
                    iniciarModoBatalla(ataquesBase, oponentes);
                    break;
                case 3: 
                    Utilidades::imprimirEnColor("Saliendo del juego. ¡Hasta pronto!", Utilidades::CYAN, false);
                    break;
                default:
                    Utilidades::imprimirEnColor("Opción inválida. Por favor, intenta de nuevo.", Utilidades::ROJO, false);
                    Utilidades::pausar();
                    break;
            }
        }
    } while (opcion != 3); 
    return 0;
}


