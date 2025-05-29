#include "Entrenador.h"
#include "Utilidades.h" 
#include <iostream>
#include <limits> 

Entrenador::Entrenador(std::string nombre) : nombre(nombre), pokemonActivoIndex(-1) {}

void Entrenador::agregarPokemon(const Pokemon& pokemon) {
    if (pokemones.size() < 6) { 
        pokemones.push_back(pokemon);
        
        pokemones.back().ultimoAtaqueUsadoIndex = -1;
        
        pokemones.back().isConscious = true;

        if (pokemonActivoIndex == -1) { 
            pokemonActivoIndex = 0;
        }
        std::cout << Utilidades::VERDE << pokemon.nombre << " se ha unido al equipo de " << nombre << "!" << Utilidades::RESET << std::endl;
    } else {
        std::cout << Utilidades::ROJO << "El equipo de " << nombre << " está lleno. No se puede añadir a " << pokemon.nombre << "." << Utilidades::RESET << std::endl;
    }
}

Pokemon& Entrenador::getPokemonActivo() {
    return pokemones[pokemonActivoIndex];
}

const Pokemon& Entrenador::getPokemonActivo() const {
    return pokemones[pokemonActivoIndex];
}


void Entrenador::cambiarPokemonActivo(int nuevoIndex) {
    if (nuevoIndex >= 0 && nuevoIndex < pokemones.size()) {
        if (pokemones[nuevoIndex].estaConsciente()) {
            pokemonActivoIndex = nuevoIndex;
            std::cout << Utilidades::CYAN << nombre << " ha enviado a " << getPokemonActivo().nombre << "!" << Utilidades::RESET << std::endl;
        } else {
            Utilidades::imprimirEnColor(pokemones[nuevoIndex].nombre + " está debilitado y no puede combatir.", Utilidades::ROJO);
        }
    } else {
        Utilidades::imprimirEnColor("Índice de Pokémon inválido.", Utilidades::ROJO);
    }
}

bool Entrenador::tienePokemonesConscientes() const {
    for (const auto& pokemon : pokemones) {
        if (pokemon.estaConsciente()) { 
            return true;
        }
    }
    return false;
}

void Entrenador::mostrarPokemones() const {
    Utilidades::dibujarSeparador('=', 40, Utilidades::AZUL);
    Utilidades::imprimirEnColor("Equipo de " + nombre + ":", Utilidades::BOLD + Utilidades::AZUL);
    Utilidades::dibujarSeparador('-', 40, Utilidades::AZUL);
    for (size_t i = 0; i < pokemones.size(); ++i) {
        std::cout << (i + 1) << ". ";
        if (static_cast<int>(i) == pokemonActivoIndex) {
            std::cout << Utilidades::BOLD << Utilidades::AMARILLO << "(ACTIVO) " << Utilidades::RESET;
        }
        pokemones[i].mostrarInfo(); 
    }
    Utilidades::dibujarSeparador('=', 40, Utilidades::AZUL);
}

void Entrenador::curarPokemones() {
    Utilidades::imprimirEnColor("\n--- Centro Pokémon ---", Utilidades::BOLD + Utilidades::VERDE);
    Utilidades::imprimirEnColor("Todos los Pokémon de " + nombre + " han sido curados!", Utilidades::VERDE);
    for (auto& pokemon : pokemones) {
        pokemon.curar(); 
    }
    Utilidades::dibujarSeparador('-', 40, Utilidades::VERDE);
    Utilidades::pausar();
}
