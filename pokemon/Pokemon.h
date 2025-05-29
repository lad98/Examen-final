#ifndef POKEMON_H
#define POKEMON_H

#include <string>   
#include <vector>   
#include "Ataque.h" 
#include "GlobalTypes.h" 
class Pokemon {
public:
    std::string nombre;    
    std::string tipo;      
    int vidaActual;       
    int vidaMaxima;        
    int ataqueStat;        
    int defensaStat;       
    std::vector<Ataque> ataques; 
    std::vector<StatusEffectType> estadosActuales;

    
    int ultimoAtaqueUsadoIndex;

    
    bool isConscious;

    
    Pokemon(std::string nombre, std::string tipo, int vida, int ataque, int defensa);


    Pokemon(const Pokemon& otro);
    
    Pokemon& operator=(const Pokemon& otro);


    
    void agregarAtaque(const Ataque& ataque);

    
    void mostrarStats() const; 

    
    void recibirDanio(int danio);

   
    bool estaConsciente() const;

   
    void curar();

    
    int usarAtaque(int indiceAtaque) const;

    
    void aplicarEstado(StatusEffectType estado);
    
    bool tieneEstado(StatusEffectType estado) const;
    void procesarTurnoEstados(); 
    void mostrarInfo() const;
};

#endif 
