/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CSound.cpp
*/

#include "engine/components/CSound.hpp"

namespace Engine {

CSound::CSound() : _sound(), _volume(1.0f), _loaded(false) {}

CSound::~CSound() { unload(); }

Sound CSound::getSound() const { return _sound; }

void CSound::unload() {
    if (_loaded)
        UnloadSound(_sound);
    _loaded = false;
}

void CSound::setSound(const std::string &soundPath) {
    unload();
    if (!FileExists(soundPath.c_str()))
        throw std::runtime_error("Sound file not found: " + soundPath);
    _sound = LoadSound(soundPath.c_str());
    _loaded = true;
}

void CSound::setVolume(float volume) { _volume = volume; }

float CSound::getVolume() const { return _volume; }
} // namespace Engine
