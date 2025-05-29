#ifndef ATAQUE_H
#define ATAQUE_H

#include <string> 
#include "GlobalTypes.h" 
class Ataque {
public:
    std::string nombre; 
    int poder;          
    std::string tipo;   
    StatusEffectType efectoSecundario;
    
    int probabilidadEfecto;

    
    Ataque(std::string nombre, int poder, std::string tipo,
           StatusEffectType efecto = StatusEffectType::None, int prob = 0);

    
    void mostrarInfo() const; 
};

#endif 
