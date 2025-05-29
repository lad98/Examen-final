#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string> 
#include <vector> 
#include <map>   

namespace Utilidades {
    const std::string RESET = "\033[0m";
    const std::string NEGRO = "\033[30m";
    const std::string ROJO = "\033[31m";
    const std::string VERDE = "\033[32m";
    const std::string AMARILLO = "\033[33m";
    const std::string AZUL = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string BLANCO = "\033[37m";

    const std::string BOLD = "\033[1m";
    const std::string SUBRAYADO = "\033[4m";
    const std::string INVERSO = "\033[7m";

 
    void limpiarPantalla();
    void pausar();
    void limpiarBufferEntrada();
    void imprimirEnColor(const std::string& texto, const std::string& color = RESET, bool negrita = false);
    void dibujarSeparador(char caracter, int longitud, const std::string& color = RESET);
    void dibujarRecuadro(const std::string& titulo, const std::string& subtitulo = "", const std::string& color = RESET, bool negrita = false);
    std::string getColorPorTipo(const std::string& tipo);

    void mostrarLogoPokemonASCII();
    
    
} 

#endif 
