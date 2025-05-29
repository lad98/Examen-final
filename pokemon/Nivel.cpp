#include "Nivel.h"       
#include "Utilidades.h"  
#include "GlobalTypes.h" 
#include "AudioManager.h" 
#include <iostream>      
#include <limits>        
#include <map>           
#include <random>        
#include <ctime>        
#include <algorithm>     


static const std::map<std::string, std::map<std::string, double>> efectividadTipos = {
    {"Normal",    {{"Roca", 0.5}, {"Fantasma", 0.0}}},
    {"Fuego",     {{"Planta", 2.0}, {"Hielo", 2.0}, {"Bicho", 2.0}, {"Acero", 2.0}, {"Agua", 0.5}, {"Roca", 0.5}, {"Dragon", 0.5}, {"Fuego", 0.5}}},
    {"Agua",      {{"Fuego", 2.0}, {"Tierra", 2.0}, {"Roca", 2.0}, {"Planta", 0.5}, {"Dragon", 0.5}, {"Agua", 0.5}}},
    {"Planta",    {{"Agua", 2.0}, {"Tierra", 2.0}, {"Roca", 2.0}, {"Fuego", 0.5}, {"Planta", 0.5}, {"Volador", 0.5}, {"Bicho", 0.5}, {"Dragon", 0.5}, {"Acero", 0.5}}},
    {"Electrico", {{"Agua", 2.0}, {"Volador", 2.0}, {"Planta", 0.5}, {"Electrico", 0.5}, {"Dragon", 0.5}, {"Tierra", 0.0}}},
    {"Roca",      {{"Fuego", 2.0}, {"Hielo", 2.0}, {"Volador", 2.0}, {"Bicho", 2.0}, {"Lucha", 0.5}, {"Tierra", 0.5}, {"Acero", 0.5}}},
    {"Tierra",    {{"Fuego", 2.0}, {"Electrico", 2.0}, {"Roca", 2.0}, {"Acero", 2.0}, {"Planta", 0.5}, {"Bicho", 0.5}, {"Volador", 0.0}}},
    {"Bicho",     {{"Planta", 2.0}, {"Psiquico", 2.0}, {"Siniestro", 2.0}, {"Fuego", 0.5}, {"Lucha", 0.5}, {"Volador", 0.5}, {"Fantasma", 0.5}, {"Acero", 0.5}}},
    {"Psiquico",  {{"Lucha", 2.0}, {"Veneno", 2.0}, {"Acero", 0.5}, {"Psiquico", 0.5}, {"Siniestro", 0.0}}},
    {"Lucha",     {{"Normal", 2.0}, {"Hielo", 2.0}, {"Roca", 2.0}, {"Acero", 2.0}, {"Siniestro", 2.0}, {"Volador", 0.5}, {"Veneno", 0.5}, {"Bicho", 0.5}, {"Psiquico", 0.5}, {"Fantasma", 0.0}}},
    {"Veneno",    {{"Planta", 2.0}, {"Hada", 2.0}, {"Veneno", 0.5}, {"Tierra", 0.5}, {"Roca", 0.5}, {"Fantasma", 0.5}}},
    {"Fantasma",  {{"Psiquico", 2.0}, {"Fantasma", 2.0}, {"Normal", 0.0}, {"Siniestro", 0.5}}},
};


std::mt19937 generador(static_cast<unsigned int>(std::time(nullptr)));


Nivel::Nivel(Entrenador& jugadorBase, const Entrenador& oponenteBase, int nivel)
    : jugador(jugadorBase), oponente(oponenteBase), numeroNivel(nivel)
{
    
    oponente.pokemonActivoIndex = -1;
}


double obtenerEfectividadTipo(const std::string& tipoAtaque, const std::string& tipoDefensor) {
    auto itAtaque = efectividadTipos.find(tipoAtaque);
    if (itAtaque != efectividadTipos.end()) {
        auto itDefensor = itAtaque->second.find(tipoDefensor);
        if (itDefensor != itAtaque->second.end()) {
            return itDefensor->second; 
        }
    }
    return 1.0; 
}


int calcularDanio(const Pokemon& atacante, const Ataque& ataque, const Pokemon& defensor) {
    double modificadorEfectividad = obtenerEfectividadTipo(ataque.tipo, defensor.tipo);

    
    double danioBase = static_cast<double>(ataque.poder) * atacante.ataqueStat / defensor.defensaStat;
    int danioFinal = static_cast<int>(danioBase * modificadorEfectividad / 2.0); 

    if (danioFinal < 1) danioFinal = 1; 

    std::cout << "La efectividad del ataque fue: ";
    if (modificadorEfectividad == 2.0) {
        Utilidades::imprimirEnColor("¡Súper efectivo!", Utilidades::VERDE, false);
        AudioManager::getInstance()->reproducirEfectoSonoro("super_efectivo"); 
    } else if (modificadorEfectividad == 0.5) {
        Utilidades::imprimirEnColor("No muy efectivo...", Utilidades::ROJO, false);
        AudioManager::getInstance()->reproducirEfectoSonoro("no_muy_efectivo"); 
    } else if (modificadorEfectividad == 0.0) {
        Utilidades::imprimirEnColor("¡No tiene efecto!", Utilidades::CYAN, false);
        AudioManager::getInstance()->reproducirEfectoSonoro("sin_efecto"); 
        return 0; 
    } else {
        std::cout << "Normal." << std::endl;
    }

    return danioFinal;
}

void Nivel::configurarEquipoOponente() {
    
    std::vector<Pokemon> equipoCompletoOponente = oponente.pokemones;

    oponente.pokemones.clear(); 
    oponente.pokemonActivoIndex = -1; 

    
    size_t numPokemonesJugador = jugador.pokemones.size();
    size_t numPokemonesOponente = numPokemonesJugador;

    
    if (numPokemonesOponente > equipoCompletoOponente.size()) {
        numPokemonesOponente = equipoCompletoOponente.size();
    }

    
    std::shuffle(equipoCompletoOponente.begin(), equipoCompletoOponente.end(), generador);

    
    for (size_t i = 0; i < numPokemonesOponente; ++i) {
        
        oponente.agregarPokemon(equipoCompletoOponente[i]);
    }

    if (!oponente.pokemones.empty()) {
        oponente.pokemonActivoIndex = 0;
    }
    Utilidades::imprimirEnColor(oponente.nombre + " ha enviado a " + oponente.getPokemonActivo().nombre + "!", Utilidades::CYAN, false);
    AudioManager::getInstance()->reproducirEfectoSonoro("enviar_pokemon"); 
    Utilidades::pausar();
}


bool Nivel::iniciarCombate() {
    configurarEquipoOponente(); 

    Utilidades::limpiarPantalla();
    Utilidades::dibujarRecuadro("NIVEL " + std::to_string(numeroNivel),
                                 jugador.nombre + " vs " + oponente.nombre,
                                 Utilidades::CYAN, true);
   
    Utilidades::pausar();

    
    while (jugador.tienePokemonesConscientes() && oponente.tienePokemonesConscientes()) {
        Utilidades::limpiarPantalla();
        Utilidades::dibujarSeparador('=', 40, Utilidades::BLANCO);
        Utilidades::imprimirEnColor("--- ESTADO DEL COMBATE ---", Utilidades::BLANCO, true);
        Utilidades::dibujarSeparador('-', 40, Utilidades::BLANCO);

        
        if (jugador.pokemonActivoIndex != -1) {
            std::cout << Utilidades::BOLD << jugador.nombre << "'s " << Utilidades::RESET;
            jugador.getPokemonActivo().mostrarInfo();
        }
        Utilidades::dibujarSeparador(' ', 40); 
        if (oponente.pokemonActivoIndex != -1) {
            std::cout << Utilidades::BOLD << oponente.nombre << "'s " << Utilidades::RESET;
            oponente.getPokemonActivo().mostrarInfo();
        }
        Utilidades::dibujarSeparador('=', 40, Utilidades::BLANCO);
        Utilidades::pausar();


        
        if (jugador.pokemonActivoIndex != -1) {
            jugador.getPokemonActivo().procesarTurnoEstados();
            if (!jugador.getPokemonActivo().estaConsciente()) {
                Utilidades::imprimirEnColor(jugador.getPokemonActivo().nombre + " se debilitó por su estado!", Utilidades::ROJO, false);
                AudioManager::getInstance()->reproducirEfectoSonoro("pokemon_fainted"); 
                if (jugador.tienePokemonesConscientes()) {
                    Utilidades::imprimirEnColor("Debes elegir otro Pokémon para continuar.", Utilidades::AMARILLO, false);
                    int eleccionPokemon;
                    bool pokemonValidoElegido = false;
                    do {
                        jugador.mostrarPokemones();
                        std::cout << "Elige un Pokémon para enviar (número): ";
                        if (!(std::cin >> eleccionPokemon)) {
                            std::cin.clear();
                            Utilidades::limpiarBufferEntrada();
                            Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
                        } else {
                            Utilidades::limpiarBufferEntrada();
                            if (eleccionPokemon > 0 && eleccionPokemon <= jugador.pokemones.size() &&
                                jugador.pokemones[eleccionPokemon - 1].estaConsciente()) {
                                if (static_cast<int>(eleccionPokemon - 1) == jugador.pokemonActivoIndex) {
                                    Utilidades::imprimirEnColor("¡Ese Pokémon ya está en combate! Elige otro.", Utilidades::ROJO, false);
                                } else {
                                    jugador.cambiarPokemonActivo(eleccionPokemon - 1);
                                    AudioManager::getInstance()->reproducirEfectoSonoro("cambio_pokemon"); 
                                    pokemonValidoElegido = true;
                                }
                            } else {
                                Utilidades::imprimirEnColor("¡Elección inválida o Pokémon debilitado! Intenta de nuevo.", Utilidades::ROJO, false);
                            }
                        }
                    } while (!pokemonValidoElegido);
                } else {
                    break; 
                }
            }
        }
        if (!jugador.tienePokemonesConscientes()) break; 
        
        turnoJugador();
        
        if (oponente.pokemonActivoIndex != -1 && !oponente.getPokemonActivo().estaConsciente()) {
            Utilidades::imprimirEnColor(oponente.getPokemonActivo().nombre + " se debilitó!", Utilidades::ROJO, false);
            AudioManager::getInstance()->reproducirEfectoSonoro("pokemon_fainted"); 
            if (oponente.tienePokemonesConscientes()) {
                for (size_t i = 0; i < oponente.pokemones.size(); ++i) {
                    if (oponente.pokemones[i].estaConsciente()) {
                        oponente.cambiarPokemonActivo(static_cast<int>(i));
                        AudioManager::getInstance()->reproducirEfectoSonoro("cambio_pokemon"); 
                        break;
                    }
                }
            } else {
                break; 
            }
        }
        Utilidades::pausar();

        if (!oponente.tienePokemonesConscientes()) break; 
        if (oponente.pokemonActivoIndex != -1) {
            oponente.getPokemonActivo().procesarTurnoEstados();
            if (!oponente.getPokemonActivo().estaConsciente()) {
                Utilidades::imprimirEnColor(oponente.getPokemonActivo().nombre + " se debilitó por su estado!", Utilidades::ROJO, false);
                AudioManager::getInstance()->reproducirEfectoSonoro("pokemon_fainted"); 
                if (oponente.tienePokemonesConscientes()) {
                    for (size_t i = 0; i < oponente.pokemones.size(); ++i) {
                        if (oponente.pokemones[i].estaConsciente()) {
                            oponente.cambiarPokemonActivo(static_cast<int>(i));
                            AudioManager::getInstance()->reproducirEfectoSonoro("cambio_pokemon"); 
                            break;
                        }
                    }
                } else {
                    break; 
                }
            }
        }
        if (!oponente.tienePokemonesConscientes()) break; 

        
        turnoOponente();
       
        if (jugador.pokemonActivoIndex != -1 && !jugador.getPokemonActivo().estaConsciente()) {
            Utilidades::imprimirEnColor(jugador.getPokemonActivo().nombre + " se debilitó!", Utilidades::ROJO, false);
            AudioManager::getInstance()->reproducirEfectoSonoro("pokemon_fainted"); 
            if (jugador.tienePokemonesConscientes()) {
                Utilidades::imprimirEnColor("Debes elegir otro Pokémon para continuar.", Utilidades::AMARILLO, false);
                int eleccionPokemon;
                bool pokemonValidoElegido = false;
                do {
                    jugador.mostrarPokemones();
                    std::cout << Utilidades::AMARILLO << "Elige un Pokémon para enviar (número): " << Utilidades::RESET;
                    if (!(std::cin >> eleccionPokemon)) {
                        std::cin.clear();
                        Utilidades::limpiarBufferEntrada();
                        Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
                    } else {
                        Utilidades::limpiarBufferEntrada();
                        if (eleccionPokemon > 0 && eleccionPokemon <= jugador.pokemones.size() &&
                            jugador.pokemones[eleccionPokemon - 1].estaConsciente()) {
                            if (static_cast<int>(eleccionPokemon - 1) == jugador.pokemonActivoIndex) {
                                Utilidades::imprimirEnColor("¡Ese Pokémon ya está en combate! Elige otro.", Utilidades::ROJO, false);
                            } else {
                                jugador.cambiarPokemonActivo(eleccionPokemon - 1);
                                AudioManager::getInstance()->reproducirEfectoSonoro("cambio_pokemon"); 
                                pokemonValidoElegido = true;
                            }
                        }
                        else {
                            Utilidades::imprimirEnColor("¡Elección inválida o Pokémon debilitado! Intenta de nuevo.", Utilidades::ROJO, false);
                        }
                    }
                } while (!pokemonValidoElegido);
            } else {
                break; 
            }
        }
        Utilidades::pausar();
    }

    
    if (jugador.tienePokemonesConscientes()) {
        Utilidades::imprimirEnColor("\n¡" + jugador.nombre + " ha ganado el Nivel " + std::to_string(numeroNivel) + "!", Utilidades::VERDE, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("victoria"); 

        
        if (!oponente.pokemones.empty() && jugador.pokemones.size() < 6) { 
            char opcionCaptura;
            Utilidades::imprimirEnColor("¿Quieres añadir un Pokémon del equipo de " + oponente.nombre + " a tu equipo? (s/n): ", Utilidades::AMARILLO, true);
            std::cin >> opcionCaptura;
            Utilidades::limpiarBufferEntrada();

            if (opcionCaptura == 's' || opcionCaptura == 'S') {
                Utilidades::dibujarSeparador('=', 40, Utilidades::AZUL);
                Utilidades::imprimirEnColor("--- Pokémon de " + oponente.nombre + " ---", Utilidades::AZUL, true);
                Utilidades::dibujarSeparador('-', 40, Utilidades::AZUL);
              
                for (size_t i = 0; i < oponente.pokemones.size(); ++i) {
                    std::cout << (i + 1) << ". ";
                    oponente.pokemones[i].mostrarInfo();
                }
                Utilidades::dibujarSeparador('=', 40, Utilidades::AZUL);

                int eleccionPokemon;
                bool eleccionValida = false;
                do {
                    std::cout << Utilidades::AMARILLO << "Elige el número del Pokémon que quieres añadir: " << Utilidades::RESET;
                    if (!(std::cin >> eleccionPokemon)) {
                        std::cin.clear();
                        Utilidades::limpiarBufferEntrada();
                        Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
                    } else {
                        Utilidades::limpiarBufferEntrada();
    
                        if (eleccionPokemon > 0 && eleccionPokemon <= oponente.pokemones.size()) {
                            Pokemon pokemonAdquirido = oponente.pokemones[eleccionPokemon - 1];
                            pokemonAdquirido.vidaActual = pokemonAdquirido.vidaMaxima; 
                            pokemonAdquirido.estadosActuales.clear(); 
                            pokemonAdquirido.ultimoAtaqueUsadoIndex = -1; 
                            pokemonAdquirido.isConscious = true; 

                            jugador.agregarPokemon(pokemonAdquirido);
                            Utilidades::imprimirEnColor("¡Has adquirido a " + pokemonAdquirido.nombre + "!", Utilidades::VERDE, false);
                            AudioManager::getInstance()->reproducirEfectoSonoro("captura_pokemon"); 
                            eleccionValida = true;
                        } else {
                            Utilidades::imprimirEnColor("¡Elección inválida! Intenta de nuevo.", Utilidades::ROJO, false);
                        }
                    }
                } while (!eleccionValida);
            } else {
                Utilidades::imprimirEnColor("No has añadido ningún Pokémon a tu equipo.", Utilidades::CYAN, false);
            }
        } else if (jugador.pokemones.size() >= 6) {
            Utilidades::imprimirEnColor("Tu equipo está lleno (máximo 6 Pokémon). No puedes añadir más Pokémon.", Utilidades::AMARILLO, false);
        }


        return true;
    } else {
        Utilidades::imprimirEnColor("\n¡" + oponente.nombre + " ha ganado el Nivel " + std::to_string(numeroNivel) + "!", Utilidades::ROJO, true);
        AudioManager::getInstance()->reproducirEfectoSonoro("derrota"); 
        return false;
    }
}


void Nivel::turnoJugador() {
    if (jugador.pokemonActivoIndex == -1) {
        Utilidades::imprimirEnColor("Error: No hay Pokémon activo para el jugador.", Utilidades::ROJO, false);
        return;
    }

    int opcion;
    Utilidades::dibujarRecuadro("TURNO DE " + jugador.nombre,
                                 "Pokémon Activo: " + jugador.getPokemonActivo().nombre + " (" +
                                 std::to_string(jugador.getPokemonActivo().vidaActual) + "/" +
                                 std::to_string(jugador.getPokemonActivo().vidaMaxima) + " HP)",
                                 Utilidades::VERDE, true);
    std::cout << "1. " << Utilidades::BOLD << "Atacar" << Utilidades::RESET << std::endl;
    std::cout << "2. " << Utilidades::BOLD << "Cambiar Pokémon" << Utilidades::RESET << std::endl;
    std::cout << Utilidades::AMARILLO << "Elige una opción: " << Utilidades::RESET;

    while (!(std::cin >> opcion) || (opcion < 1 || opcion > 2)) {
        std::cin.clear();
        Utilidades::limpiarBufferEntrada();
        Utilidades::imprimirEnColor("¡Opción inválida! Por favor, ingresa 1 o 2: ", Utilidades::ROJO, false);
    }
    Utilidades::limpiarBufferEntrada();

    switch (opcion) {
        case 1: {
            if (jugador.getPokemonActivo().ataques.empty()) {
                Utilidades::imprimirEnColor(jugador.getPokemonActivo().nombre + " no tiene ataques y no puede atacar!", Utilidades::ROJO, false);
                return;
            }
            Utilidades::dibujarSeparador('~', 40, Utilidades::AMARILLO);
            Utilidades::imprimirEnColor("Elige un ataque para " + jugador.getPokemonActivo().nombre + ":", Utilidades::AMARILLO, true);
            Utilidades::dibujarSeparador('-', 40, Utilidades::AMARILLO);
            for (size_t i = 0; i < jugador.getPokemonActivo().ataques.size(); ++i) {
                std::cout << (i + 1) << ". ";
                jugador.getPokemonActivo().ataques[i].mostrarInfo();
            }
            Utilidades::dibujarSeparador('~', 40, Utilidades::AMARILLO);
            int eleccionAtaque;
            std::cout << Utilidades::CYAN << "Tu elección: " << Utilidades::RESET;

            while (!(std::cin >> eleccionAtaque) ||
                   (eleccionAtaque < 1 || eleccionAtaque > jugador.getPokemonActivo().ataques.size())) {
                std::cin.clear();
                Utilidades::limpiarBufferEntrada();
                Utilidades::imprimirEnColor("¡Ataque inválido! Por favor, elige un número válido: ", Utilidades::ROJO, false);
            }
            Utilidades::limpiarBufferEntrada();

            if (oponente.pokemonActivoIndex != -1) {
                const Ataque& ataqueUsado = jugador.getPokemonActivo().ataques[eleccionAtaque - 1];
                Utilidades::imprimirEnColor(jugador.getPokemonActivo().nombre + " usó " + ataqueUsado.nombre + "!", Utilidades::VERDE, false);
                AudioManager::getInstance()->reproducirEfectoSonoro("ataque_generico"); 
               
                int danio = calcularDanio(jugador.getPokemonActivo(), ataqueUsado, oponente.getPokemonActivo());
                oponente.getPokemonActivo().recibirDanio(danio);
                std::cout << oponente.getPokemonActivo().nombre << " recibió " << danio << " de daño." << std::endl;

    
                if (ataqueUsado.efectoSecundario != StatusEffectType::None) {
                    std::uniform_int_distribution<> dist(1, 100); 
                    if (dist(generador) <= ataqueUsado.probabilidadEfecto) {
                        oponente.getPokemonActivo().aplicarEstado(ataqueUsado.efectoSecundario);
                        AudioManager::getInstance()->reproducirEfectoSonoro("efecto_estado"); 
                    }
                }
            } else {
                Utilidades::imprimirEnColor("El oponente no tiene un Pokémon activo para atacar.", Utilidades::ROJO, false);
            }
            break;
        }
        case 2: {
            int eleccionPokemon;
            bool pokemonValidoElegido = false;
            do {
                jugador.mostrarPokemones();
                std::cout << Utilidades::AMARILLO << "Elige un Pokémon para enviar (número): " << Utilidades::RESET;
                if (!(std::cin >> eleccionPokemon)) {
                    std::cin.clear();
                    Utilidades::limpiarBufferEntrada();
                    Utilidades::imprimirEnColor("¡Entrada inválida! Por favor, ingresa un número.", Utilidades::ROJO, false);
                } else {
                    Utilidades::limpiarBufferEntrada();
                    if (eleccionPokemon > 0 && eleccionPokemon <= jugador.pokemones.size() &&
                        jugador.pokemones[eleccionPokemon - 1].estaConsciente()) {
                        if (static_cast<int>(eleccionPokemon - 1) == jugador.pokemonActivoIndex) {
                            Utilidades::imprimirEnColor("¡Ese Pokémon ya está en combate! Elige otro.", Utilidades::ROJO, false);
                        } else {
                            jugador.cambiarPokemonActivo(eleccionPokemon - 1);
                            AudioManager::getInstance()->reproducirEfectoSonoro("cambio_pokemon"); 
                            pokemonValidoElegido = true;
                        }
                    }
                    else {
                        Utilidades::imprimirEnColor("¡Elección inválida o Pokémon debilitado! Intenta de nuevo.", Utilidades::ROJO, false);
                    }
                }
            } while (!pokemonValidoElegido);
            break;
        }
    }
}


void Nivel::turnoOponente() {
    Utilidades::dibujarRecuadro("TURNO DE " + oponente.nombre,
                                 "Pokémon Activo: " + oponente.getPokemonActivo().nombre + " (" +
                                 std::to_string(oponente.getPokemonActivo().vidaActual) + "/" +
                                 std::to_string(oponente.getPokemonActivo().vidaMaxima) + " HP)",
                                 Utilidades::ROJO, true);

    if (oponente.pokemonActivoIndex == -1 || !oponente.getPokemonActivo().estaConsciente()) {
        Utilidades::imprimirEnColor("El oponente no tiene un Pokémon activo para atacar.", Utilidades::ROJO, false);
        return;
    }

    if (oponente.getPokemonActivo().ataques.empty()) {
        Utilidades::imprimirEnColor(oponente.getPokemonActivo().nombre + " no tiene ataques y no puede hacer nada!", Utilidades::ROJO, false);
        return;
    }

    if (jugador.pokemonActivoIndex != -1) {
        
        int ataqueIndex = (oponente.getPokemonActivo().ultimoAtaqueUsadoIndex + 1) % oponente.getPokemonActivo().ataques.size();
        oponente.getPokemonActivo().ultimoAtaqueUsadoIndex = ataqueIndex; 

        const Ataque& ataqueUsado = oponente.getPokemonActivo().ataques[ataqueIndex];
        Utilidades::imprimirEnColor(oponente.getPokemonActivo().nombre + " usó " + ataqueUsado.nombre + "!", Utilidades::ROJO, false);
        AudioManager::getInstance()->reproducirEfectoSonoro("ataque_generico"); 
        int danio = calcularDanio(oponente.getPokemonActivo(), ataqueUsado, jugador.getPokemonActivo());
        jugador.getPokemonActivo().recibirDanio(danio);
        std::cout << jugador.getPokemonActivo().nombre << " recibió " << danio << " de daño." << std::endl;

        
        if (ataqueUsado.efectoSecundario != StatusEffectType::None) {
            std::uniform_int_distribution<> dist(1, 100);
            if (dist(generador) <= ataqueUsado.probabilidadEfecto) {
                jugador.getPokemonActivo().aplicarEstado(ataqueUsado.efectoSecundario);
                AudioManager::getInstance()->reproducirEfectoSonoro("efecto_estado"); 
            }
        }
    } else {
        Utilidades::imprimirEnColor("El jugador no tiene un Pokémon activo para recibir el ataque.", Utilidades::ROJO, false);
    }
}

