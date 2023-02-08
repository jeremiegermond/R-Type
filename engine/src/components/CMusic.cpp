/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CMusic.cpp
*/

#include "engine/components/CMusic.hpp"

namespace Engine {
    CMusic::CMusic() : _music(nullptr) {}

    CMusic::~CMusic() {
        if (_music != nullptr) {
            // StopMusicStream(*_music);
            // UnloadMusicStream(*_music);
        }
    }

    std::shared_ptr<Music> CMusic::getMusic() const { return _music; }

    void CMusic::setMusic(const std::string &musicPath) {
        if (_music) {
            StopMusicStream(*_music);
            UnloadMusicStream(*_music);
            _music = nullptr;
        }
        if (!FileExists(musicPath.c_str()))
            throw std::runtime_error("Music file not found: " + musicPath);
        _music = std::make_shared<Music>(LoadMusicStream(musicPath.c_str()));
    }

    void CMusic::setLoop(bool loop) { _music->looping = loop; }
}
