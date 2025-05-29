#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <map>
#include <iostream> 
class AudioManager {
private:
    
    static AudioManager* instance;

    
    AudioManager();

    
    std::map<std::string, std::string> soundEffects;

    
    bool audioEnabled;

public:
   
    ~AudioManager();

    
    static AudioManager* getInstance();

    static void destroyInstance();

    void toggleAudio(bool enable);

    void cargarEfectoSonoro(const std::string& nombre, const std::string& onomatopella);

    void reproducirEfectoSonoro(const std::string& nombre);

    
    bool isAudioEnabled() const { return audioEnabled; }
};

#endif 
