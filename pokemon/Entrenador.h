#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include <string>   
#include <vector>  
#include "Pokemon.h" 

class Entrenador {
public:
    std::string nombre;
    std::vector<Pokemon> pokemones;
    
    int pokemonActivoIndex;

    
    Entrenador(std::string nombre);

    
    void agregarPokemon(const Pokemon& pokemon);
    void mostrarPokemones() const;
    void cambiarPokemonActivo(int indice); 
    bool tienePokemonesConscientes() const;

    
    void curarPokemones();

    
    Pokemon& getPokemonActivo();
    const Pokemon& getPokemonActivo() const; 
};

#endif 
