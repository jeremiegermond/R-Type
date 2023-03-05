/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CSound.cpp
*/

#include "engine/components/CSound.hpp"

namespace Engine {

    /**
     * It's the constructor for the CSound class
     */
    CSound::CSound() : _sound(), _volume(1.0f), _loaded(false) {}

    /**
     * The destructor for the CSound class.
     */
    CSound::~CSound() { unload(); }

    /**
     * It returns the sound of the object.
     * 
     * @return A reference to the private member _sound.
     */
    Sound CSound::getSound() const { return _sound; }

    /**
     * If the sound is loaded, unload it
     */
    void CSound::unload() {
        if (_loaded)
            UnloadSound(_sound);
        _loaded = false;
    }

    /**
     * It takes a string as an argument, and if the file exists, it loads the sound
     * 
     * @param soundPath The path to the sound file.
     */
    void CSound::setSound(const std::string &soundPath) {
        unload();
        if (!FileExists(soundPath.c_str()))
            throw std::runtime_error("Sound file not found: " + soundPath);
        _sound = LoadSound(soundPath.c_str());
        _loaded = true;
    }

    /**
     * This function sets the volume of the sound.
     * 
     * @param volume The volume of the sound.
     */
    void CSound::setVolume(float volume) { _volume = volume; }

    /**
     * The function returns the volume of the sound
     * 
     * @return The volume of the sound.
     */
    float CSound::getVolume() const { return _volume; }
}
