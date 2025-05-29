
#include "Utilidades.h"
#include <iostream>  
#include <limits>    
#include <map>       


namespace Utilidades {

    
    static const std::map<std::string, std::string> coloresPorTipo = {
        {"Normal", BLANCO},
        {"Fuego", ROJO},
        {"Agua", AZUL},
        {"Planta", VERDE},
        {"Electrico", AMARILLO},
        {"Roca", NEGRO}, 
        {"Tierra", AMARILLO}, 
        {"Bicho", VERDE}, 
        {"Psiquico", MAGENTA},
        {"Lucha", ROJO},
        {"Veneno", MAGENTA},
        {"Fantasma", AZUL}, 
        {"Siniestro", NEGRO},
        {"Hada", MAGENTA}, 
        {"Hielo", CYAN},
        {"Dragon", MAGENTA}, 
        {"Acero", BLANCO} 
    };

    void limpiarPantalla() {
        
        #ifdef _WIN32
            system("cls");
        
        #else
            system("clear");
        #endif
    }

    void pausar() {
        std::cout << "\nPresiona Enter para continuar..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        
    }

    void limpiarBufferEntrada() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void imprimirEnColor(const std::string& texto, const std::string& color, bool negrita) {
        if (negrita) {
            std::cout << BOLD << color << texto << RESET << std::endl;
        } else {
            std::cout << color << texto << RESET << std::endl;
        }
    }

    void dibujarSeparador(char caracter, int longitud, const std::string& color) {
        std::cout << color;
        for (int i = 0; i < longitud; ++i) {
            std::cout << caracter;
        }
        std::cout << RESET << std::endl;
    }

    
    void dibujarRecuadro(const std::string& titulo, const std::string& subtitulo, const std::string& color, bool negrita) {
        int ancho = 50; 
        dibujarSeparador('=', ancho, color);
        imprimirEnColor("| " + titulo + std::string(ancho - titulo.length() - 4, ' ') + " |", color, negrita); 
        if (!subtitulo.empty()) {
            imprimirEnColor("| " + subtitulo + std::string(ancho - subtitulo.length() - 4, ' ') + " |", color, negrita); 
        }
        dibujarSeparador('=', ancho, color);
    }

    std::string getColorPorTipo(const std::string& tipo) {
        auto it = coloresPorTipo.find(tipo);
        if (it != coloresPorTipo.end()) {
            return it->second;
        }
        return BLANCO; 
    }

    
    void mostrarLogoPokemonASCII() {
        std::cout << AZUL << R"(
                                                                                                            
                                                                                                            
                                              @@@@                                                          
                                             @@#+@@                                                         
                                           @@@- .+@@                                                        
                        @@@@@@@@           @@@@@@@@@@ @@@@- .+@@ @@@@@@@@@@@@                               
                     @@@@#*+=--==+%@@    @@@@*=-:#@@=-#@@@@@@@%%@@@@@@%===#@:..@@       @@@@@@@@            
                   @@#+:..        ..-%@@ @@@%#. .*%: ..-@@#-.....:+@@@=  .-%  .*@      @@@=::-=@@@@@@@@@    
                   @@*.      .:--: ..@@ @@@@@: .-.  .:#@= .#@@#.:*@@@:    .=  .=@@@@@@@@%    .*@@:.:=@@    
                    @@#.:.   .+@*@- .@@@@@@@@@=    .*@@%  +@@+.=@@@@%      ..    :@@#=:.-*@-. .-@#  .+@@    
                     @@@@* .@%@- +@%::+...=@:    .#@@@@. .+..*=..=@@: .      .%*.*%..:.:@:  .%-  :@@      
                       @@@@+    .=#:.#@+.:@#.:+.=%    ...:-#%:    ..  :#@+. :#..-.*#..+@@%#..@=. .-. .#@    
                         @@@@-    .:*@@%. .+%@#:.=@ .%#+:...=#%#**##@@@@. .+@*-@:*# ..:-:..=@.+ ..  =@@    
                          @@@@:  .-@@@@.    .... :%+ .%@@@@*=:..:#@@@@@@%+==%@@@@=:%+.    .:*%:.#.    .%@    
                           @@@%.  .#@@@#:....:+@@. .@@@@@@@@%+-#@ @@@@@@@@@@@@*.:+%%##@@%  :@.  .*@    
                            @@@#. .-@@@@@@@@@@@@@%@@@@    @@@@@@@        @@@@@@@@@@ @@@*+*@:  :@@    
                              @@@+  .%@ @@@@@ @@@@@@        @@@          @@@@@@@  @@@@@@@: .#@      
                              @@@@#%@@@                                           @@@@@%@@@@      
                               @@@@@@                                              @@@@@          
                                                                                                            
                                                                                                            
                                                                                                            
                                                                                                            
                                                                                                            
                                                                                                            
        )" << RESET << std::endl;
    }
}

    
    
