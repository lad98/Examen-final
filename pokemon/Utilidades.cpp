// Utilidades.cpp
#include "Utilidades.h"
#include <iostream>  // Para std::cout, std::cin
#include <limits>    // Para std::numeric_limits
#include <map>       // Para std::map

// Implementación de las funciones de utilidad dentro del namespace Utilidades.
namespace Utilidades {

    // Mapa para almacenar los colores por tipo de Pokémon
    // Esto es privado dentro del namespace Utilidades o se inicializa una vez.
    // Lo inicializamos aquí para que esté disponible globalmente para las funciones de Utilidades.
    static const std::map<std::string, std::string> coloresPorTipo = {
        {"Normal", BLANCO},
        {"Fuego", ROJO},
        {"Agua", AZUL},
        {"Planta", VERDE},
        {"Electrico", AMARILLO},
        {"Roca", NEGRO}, // O un gris oscuro si lo definimos
        {"Tierra", AMARILLO}, // Un amarillo más oscuro
        {"Bicho", VERDE}, // Un verde diferente
        {"Psiquico", MAGENTA},
        {"Lucha", ROJO},
        {"Veneno", MAGENTA},
        {"Fantasma", AZUL}, // Un azul más oscuro/grisáceo
        {"Siniestro", NEGRO},
        {"Hada", MAGENTA}, // Un magenta más claro
        {"Hielo", CYAN},
        {"Dragon", MAGENTA}, // Otro magenta/púrpura
        {"Acero", BLANCO} // Un blanco/gris metálico
    };

    void limpiarPantalla() {
        // En sistemas Windows
        #ifdef _WIN32
            system("cls");
        // En sistemas basados en Unix (Linux, macOS)
        #else
            system("clear");
        #endif
    }

    void pausar() {
        std::cout << "\nPresiona Enter para continuar..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpia cualquier entrada previa
        //std::cin.get(); // Espera a que el usuario presione Enter
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

    // CORREGIDO: Implementación de dibujarRecuadro con el parámetro 'negrita'
    void dibujarRecuadro(const std::string& titulo, const std::string& subtitulo, const std::string& color, bool negrita) {
        int ancho = 50; // Ancho fijo para el recuadro
        dibujarSeparador('=', ancho, color);
        imprimirEnColor("| " + titulo + std::string(ancho - titulo.length() - 4, ' ') + " |", color, negrita); // Usa 'negrita' aquí
        if (!subtitulo.empty()) {
            imprimirEnColor("| " + subtitulo + std::string(ancho - subtitulo.length() - 4, ' ') + " |", color, negrita); // Usa 'negrita' aquí
        }
        dibujarSeparador('=', ancho, color);
    }

    std::string getColorPorTipo(const std::string& tipo) {
        auto it = coloresPorTipo.find(tipo);
        if (it != coloresPorTipo.end()) {
            return it->second;
        }
        return BLANCO; // Color por defecto si el tipo no se encuentra
    }

    // Implementación de la función para el logo ASCII de Pokémon
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

    // Nueva función para mostrar una Pokeball ASCII
    void mostrarPokeballASCII() {
        std::cout << ROJO << R"(
     .--.
    /o  o\
   |      |
   \  --  /
    `----`
)" << RESET << std::endl;
    }

    // Nueva función para mostrar un efecto de curación ASCII
    void mostrarEfectoCuracionASCII() {
        std::cout << VERDE << R"(
                               ..                                     .......                           
                         ..:=*%@@@@@@@#+-:.                      .:-+#@@@@@@@%*=-..                     
                     .. =@@@@@@@@@@@@@@@@@@@@*:...               .:*@@@@@@@@@@@@@@@@@@@@=..                 
                   .*@@@@@@@@@@@@@@@@@@@@@@@@@@#..         .#@@@@@@@@@@@@@@@@@@@@@@@@@@*.               
                .-@@@@@@@@@%=:..     ..%@@@@@@@@@@:      :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:.            
               :@@@@@@@@=..           .+@@@@@@@@@@@#.  .#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%:           
             .*@@@@@@#:..  .:+#@@@@@@@@@@@@@@@@@@@@@@==@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*.         
            .%@@@@@#..  .=%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%..       
           :@@@@@@-.  .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:       
          .%@@@@@:  .-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.      
         .*@@@@@.   +@@@@@@@@@@@@@@@@@@@@@@@@@@*...        .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#.     
         :@@@@@=  .*@@@@@@@@@@@@@@@@@@@@@@@@@@....          ..%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-     
         *@@@@%.  -@@@@@@@@@@@@@@@@@@@@@@@@@@=...*########*.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#.    
        .@@@@@-.  *@@@@@@@@@@@@@@@@@@@@@@@@@@=...%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.    
        :@@@@@.  .@@@@@@@@@@@@@@@@@@@@@@@@@@@=...%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:    
        :@@@@@.  .@@@@@@@@@@@@@@@@@@@@@@@@@@@=...%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:    
        :@@@@@.  .@@@@@@@@@@@@@@@@@@@@@@@@@@@=...%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:    
        .@@@@@=   +@@@@@@@@@@@@@@@@@@@@@@@@@@=  .%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.    
         *@@@@@.  :@@@@@@@@@@@@@@-:...........  .%@@@@@@@@%.  ...........:-@@@@@@@@@@@@@@@@@@@@@@@+     
         :@@@@@=...*@@@@@@@@@@@%.              ..%@@@@@@@@%.               .%@@@@@@@@@@@@@@@@@@@@@:     
         .#@@@@%....#@@@@@@@@@@+  .%@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%.  =@@@@@@@@@@@@@@@@@@@@#.     
         .:@@@@@%.  :@@@@@@@@@@=  .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.  =@@@@@@@@@@@@@@@@@@@@:      
           =@@@@@+.  .@@@@@@@@@=  .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.  =@@@@@@@@@@@@@@@@@@@-       
           .#@@@@@*.  .@@@@@@@@=  .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.  =@@@@@@@@@@@@@@@@@@*          
              .*@@@@@@- ..:@@@@@:.              .%@@@@@@@@%.              .-@@@@@@@@@@@@@@@@*            
            .=@@@@@@%.   -@@@@@@@@@@@@@@@=  .%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@@@-.            
              .%@@@@@@=.  .+@@@@@@@@@@@@@=  .%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@@%..             
                +@@@@@@@:.  .#@@@@@@@@@@@=  .%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@@@+...             
                ..%@@@@@@#      ..:@@@@@@@@@@=  .%@@@     @@@@@%.  =@@@@@@@@@@@@@@@@@@@@@@%..                
                  .-@@@@@@@*.   :@@@@@@@@=  .%@@@@@@@@%.  =@@@@@@@@@@@@@@@@@@@@@-.                  
                    .*@@@@@@@*.. .:@@@@@@+  .----------.  =@@@@@@@@@@@@@@@@@@@*..                   
                     ..#@@@@@@@+.     .-%@@@@-.                   :@@@@@@@@@@@@@@@@@@#.                      
                        .%@@@@@@@=.  .:%@@@@+===========%@@@@@@@@@@@@@@@@@%:.                       
                          :%@@@@@@@=.   .=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%:                          
                            :@@@@@@@@+..%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:..                          
                             .-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:..                            
                               .:@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%:.                               
                                 .:#@@@@@@@@@@@@@@@@@@@@@@@@@@@@*:...                               
                                   ..#@@@@@@@@@@@@@@@@@@@@@@@@+....                                 
                                      .+@@@@@@@@@@@@@@@@@@@@=                                       
                                        .-@@@@@@@@@@@@@@@%:.                                        
                                           .%@@@@@@@@@@*...                                         
                                            ..*@@@@@@+..                                            
                                               .-%%-.                                               
                                                                                                            
                                                                                                            
                                                                                                            
                                                                                                            
                                                                                                            
        )" << RESET << std::endl;
    }

} // Fin del namespace Utilidades // Fin del namespace Utilidades
