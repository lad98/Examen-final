#ifndef HISTORIA_H
#define HISTORIA_H

#include <vector>
#include <string>
#include "Entrenador.h" 
#include "Nivel.h"      
#include "Utilidades.h" 
class Historia {
private:
    Entrenador& jugador; 
    std::vector<Entrenador>& oponentes; 
    bool historiaCompleta; 

    
    void mostrarNarrativa(const std::string& texto);
    int obtenerEleccion(const std::vector<std::string>& opciones);

    
    void introduccion();
    void capituloCiudadInicial();
    void iniciarNiveles(); 

public:
   
    Historia(Entrenador& jugadorRef, std::vector<Entrenador>& oponentesRef);

    
    void iniciarAventura();

    
    bool esHistoriaCompleta() const;
};

#endif 
