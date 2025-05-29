#ifndef NIVEL_H
#define NIVEL_H

#include "Entrenador.h" 
#include <vector>       


class Nivel {
public:
    Entrenador& jugador;   
    Entrenador oponente;   
    int numeroNivel;       

    
    Nivel(Entrenador& jugadorBase, const Entrenador& oponenteBase, int nivel);


    bool iniciarCombate();

private:
   
    void turnoJugador();
    void turnoOponente();

    void configurarEquipoOponente();
};

#endif 
