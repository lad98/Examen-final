
#include "Ataque.h"
#include "Utilidades.h" 
#include <iostream>

Ataque::Ataque(std::string nombre, int poder, std::string tipo, StatusEffectType efecto, int probabilidad)
    : nombre(nombre), poder(poder), tipo(tipo), efectoSecundario(efecto), probabilidadEfecto(probabilidad) {}

void Ataque::mostrarInfo() const {
    std::cout << Utilidades::BOLD << nombre << Utilidades::RESET << " (Tipo: "
              << Utilidades::getColorPorTipo(tipo) << tipo << Utilidades::RESET << ", Poder: "
              << poder << ")";
    if (efectoSecundario != StatusEffectType::None) {
        std::cout << " [Efecto: ";
        switch(efectoSecundario) {
            case StatusEffectType::Poisoned: std::cout << Utilidades::MAGENTA << "Envenenar" << Utilidades::RESET; break;
            case StatusEffectType::Burned: std::cout << Utilidades::ROJO << "Quemar" << Utilidades::RESET; break; 
            case StatusEffectType::Paralyzed: std::cout << Utilidades::AMARILLO << "Paralizar" << Utilidades::RESET; break; 
            case StatusEffectType::Asleep: std::cout << Utilidades::AZUL << "Dormir" << Utilidades::RESET; break; 
            case StatusEffectType::Frozen: std::cout << Utilidades::CYAN << "Congelar" << Utilidades::RESET; break; 
            case StatusEffectType::None: /* No debería llegar aquí */ break;
        }
        std::cout << " " << probabilidadEfecto << "%]";
    }
    std::cout << std::endl;
}
