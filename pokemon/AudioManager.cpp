#include "AudioManager.h"
#include <iostream> 
#include <algorithm> 
AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() : audioEnabled(true) {
    std::cout << "AudioManager: Inicializando sistema de 'audio' (texto)..." << std::endl;

    
    cargarEfectoSonoro("victoria", "¡YAY! 🎉");
    cargarEfectoSonoro("derrota", "¡Oh no! 😭");
    cargarEfectoSonoro("pokemon_elegido", "¡Pika! ⚡");
    cargarEfectoSonoro("inicio_juego", "¡Ding-dong! 🔔");
    cargarEfectoSonoro("super_efectivo", "¡BOOM! 💥");
    cargarEfectoSonoro("no_muy_efectivo", "¡Zzz... 😴");
    cargarEfectoSonoro("sin_efecto", "¡Nulo! 🚫");
    cargarEfectoSonoro("enviar_pokemon", "¡GO! ✨");
    cargarEfectoSonoro("pokemon_fainted", "¡Fshhh! 💨");
    cargarEfectoSonoro("cambio_pokemon", "¡Swish! 🔄");
    cargarEfectoSonoro("ataque_generico", "¡Pew! ✨");
    cargarEfectoSonoro("efecto_estado", "¡Tsssk! 🦠");
    cargarEfectoSonoro("captura_pokemon", "¡Cling! 🌟");
}


AudioManager::~AudioManager() {
    std::cout << "AudioManager: Limpiando 'recursos' de audio (texto)..." << std::endl;
    soundEffects.clear(); 
}


AudioManager* AudioManager::getInstance() {
    if (instance == nullptr) {
        instance = new AudioManager();
    }
    return instance;
}


void AudioManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}


void AudioManager::toggleAudio(bool enable) {
    audioEnabled = enable;
    std::cout << "AudioManager: Audio " << (audioEnabled ? "habilitado" : "deshabilitado") << " (texto)." << std::endl;
}


void AudioManager::cargarEfectoSonoro(const std::string& nombre, const std::string& onomatopella) {
    if (soundEffects.find(nombre) == soundEffects.end()) {
        soundEffects[nombre] = onomatopella;
        std::cout << "AudioManager: Efecto de sonido '" << nombre << "' cargado como '" << onomatopella << "'" << std::endl;
    } else {
        std::cout << "AudioManager: El efecto de sonido '" << nombre << "' ya está cargado." << std::endl;
    }
}


void AudioManager::reproducirEfectoSonoro(const std::string& nombre) {
    if (!audioEnabled) {
       
        return;
    }
    if (soundEffects.count(nombre)) {
        std::cout << soundEffects[nombre] << std::endl; 
    } else {
        std::cerr << "AudioManager ERROR: Efecto de sonido '" << nombre << "' no encontrado." << std::endl;
    }
}



