
#include "Pokemon.h"
#include "Utilidades.h" 
#include <iostream>
#include <algorithm> 
Pokemon::Pokemon(std::string nombre, std::string tipo, int vida, int ataque, int defensa)
    : nombre(nombre), tipo(tipo), vidaMaxima(vida), vidaActual(vida), ataqueStat(ataque), defensaStat(defensa),
      isConscious(true), ultimoAtaqueUsadoIndex(-1)
{
    
    estadosActuales.clear();
}


Pokemon::Pokemon(const Pokemon& otro)
    : nombre(otro.nombre), tipo(otro.tipo),
      vidaMaxima(otro.vidaMaxima), vidaActual(otro.vidaActual),
      ataqueStat(otro.ataqueStat), defensaStat(otro.defensaStat),
      isConscious(otro.isConscious), ataques(otro.ataques),
      estadosActuales(otro.estadosActuales), ultimoAtaqueUsadoIndex(otro.ultimoAtaqueUsadoIndex)
{
}


Pokemon& Pokemon::operator=(const Pokemon& otro) {
    if (this != &otro) {
        nombre = otro.nombre;
        tipo = otro.tipo;
        vidaMaxima = otro.vidaMaxima;
        vidaActual = otro.vidaActual;
        ataqueStat = otro.ataqueStat;
        defensaStat = otro.defensaStat;
        isConscious = otro.isConscious; 
        ataques = otro.ataques;
        estadosActuales = otro.estadosActuales;
        ultimoAtaqueUsadoIndex = otro.ultimoAtaqueUsadoIndex;
    }
    return *this;
}

void Pokemon::agregarAtaque(const Ataque& ataque) {
    if (ataques.size() < 4) { 
        ataques.push_back(ataque);
    } else {
        std::cout << nombre << " ya tiene 4 ataques y no puede aprender más." << std::endl;
    }
}

void Pokemon::recibirDanio(int danio) {
    vidaActual -= danio;
    if (vidaActual <= 0) {
        vidaActual = 0;
        isConscious = false; 
        std::cout << Utilidades::ROJO << nombre << " se ha debilitado!" << Utilidades::RESET << std::endl;
    } else {
        std::cout << nombre << " tiene " << Utilidades::VERDE << vidaActual << "/" << vidaMaxima << Utilidades::RESET << " HP restantes." << std::endl;
    }
}

bool Pokemon::estaConsciente() const {
    return isConscious; 
}

void Pokemon::curar() {
    vidaActual = vidaMaxima;
    isConscious = true; 
    estadosActuales.clear(); 
    ultimoAtaqueUsadoIndex = -1; 
    std::cout << Utilidades::VERDE << nombre << " ha sido curado completamente!" << Utilidades::RESET << std::endl;
}

void Pokemon::aplicarEstado(StatusEffectType estado) {
    
    for (StatusEffectType existing_state : estadosActuales) {
        if (existing_state == estado) {
            std::cout << nombre << " ya está afectado por este estado." << std::endl;
            return;
        }
    }

    estadosActuales.push_back(estado);
    switch (estado) {
        case StatusEffectType::Poisoned:
            Utilidades::imprimirEnColor(nombre + " ha sido envenenado!", Utilidades::MAGENTA);
            break;
        case StatusEffectType::Burned: 
            Utilidades::imprimirEnColor(nombre + " ha sido quemado!", Utilidades::ROJO);
            break;
        case StatusEffectType::Paralyzed: 
            Utilidades::imprimirEnColor(nombre + " ha sido paralizado!", Utilidades::AMARILLO);
            break;
        case StatusEffectType::Asleep: 
            Utilidades::imprimirEnColor(nombre + " se ha dormido!", Utilidades::AZUL);
            break;
        case StatusEffectType::Frozen: 
            Utilidades::imprimirEnColor(nombre + " se ha congelado!", Utilidades::CYAN);
            break;
        default:
            break;
    }
}

void Pokemon::procesarTurnoEstados() {
    if (!isConscious) return; 
    estadosActuales.erase(
        std::remove_if(estadosActuales.begin(), estadosActuales.end(),
            [](StatusEffectType s){
                return s == StatusEffectType::Frozen || s == StatusEffectType::Asleep; 
            }),
        estadosActuales.end()
    );


    for (StatusEffectType estado : estadosActuales) {
        switch (estado) {
            case StatusEffectType::Poisoned: {
                int danioVeneno = static_cast<int>(vidaMaxima * 0.05); 
                vidaActual -= danioVeneno;
                Utilidades::imprimirEnColor(nombre + " está sufriendo por el veneno! (-" + std::to_string(danioVeneno) + " HP)", Utilidades::MAGENTA);
                if (vidaActual <= 0) {
                    vidaActual = 0;
                    isConscious = false; 
                    Utilidades::imprimirEnColor(nombre + " se ha debilitado por el veneno!", Utilidades::ROJO);
                }
                break;
            }
            case StatusEffectType::Burned: { 
                int danioQuemadura = static_cast<int>(vidaMaxima * 0.05); 
                vidaActual -= danioQuemadura;
                Utilidades::imprimirEnColor(nombre + " está sufriendo por la quemadura! (-" + std::to_string(danioQuemadura) + " HP)", Utilidades::ROJO);
                if (vidaActual <= 0) {
                    vidaActual = 0;
                    isConscious = false; 
                    Utilidades::imprimirEnColor(nombre + " se ha debilitado por la quemadura!", Utilidades::ROJO);
                }
                break;
            }
            
            default:
                break;
        }
    }
}

bool Pokemon::tieneEstado(StatusEffectType estado) const {
    for (StatusEffectType s : estadosActuales) {
        if (s == estado) {
            return true;
        }
    }
    return false;
}

void Pokemon::mostrarInfo() const {
    std::cout << Utilidades::getColorPorTipo(tipo) << Utilidades::BOLD << nombre << Utilidades::RESET;
    std::cout << " (Tipo: " << Utilidades::getColorPorTipo(tipo) << tipo << Utilidades::RESET << ") ";
    std::cout << "HP: " << Utilidades::VERDE << vidaActual << "/" << vidaMaxima << Utilidades::RESET << " ";
    std::cout << "ATK: " << ataqueStat << " DEF: " << defensaStat;
    if (!estadosActuales.empty()) {
        std::cout << " [Estados: ";
        for (size_t i = 0; i < estadosActuales.size(); ++i) {
            switch (estadosActuales[i]) {
                case StatusEffectType::Poisoned: std::cout << Utilidades::MAGENTA << "ENV" << Utilidades::RESET; break;
                case StatusEffectType::Burned: std::cout << Utilidades::ROJO << "QUE" << Utilidades::RESET; break; 
                case StatusEffectType::Paralyzed: std::cout << Utilidades::AMARILLO << "PAR" << Utilidades::RESET; break; 
                case StatusEffectType::Asleep: std::cout << Utilidades::AZUL << "DOR" << Utilidades::RESET; break; 
                case StatusEffectType::Frozen: std::cout << Utilidades::CYAN << "CON" << Utilidades::RESET; break; 
                case StatusEffectType::None: /* No debería ocurrir */ break;
            }
            if (i < estadosActuales.size() - 1) std::cout << ",";
        }
        std::cout << "]";
    }
    std::cout << std::endl;

    if (!ataques.empty()) {
        std::cout << "  Ataques: ";
        for (size_t i = 0; i < ataques.size(); ++i) {
            std::cout << ataques[i].nombre;
            if (i < ataques.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

