/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CMusic.cpp
*/

#include "engine/components/CMusic.hpp"

namespace Engine {
    /**
     * CMusic::CMusic() : _music(nullptr) {}
     */
    CMusic::CMusic() : _music(nullptr) {}

    /**
     * CMusic::~CMusic() { unload(); }
     */
    CMusic::~CMusic() { unload(); }

    /**
     * If the music is not null, stop the music, unload the music, and set the music to null
     */
    void CMusic::unload() {
        if (_music != nullptr) {
            StopMusicStream(*_music);
            UnloadMusicStream(*_music);
            _music = nullptr;
        }
    }

    /**
     * It returns a shared pointer to a music object.
     * 
     * @return A shared pointer to a Music object.
     */
    std::shared_ptr<Music> CMusic::getMusic() const { return _music; }

    /**
     * "If the file exists, unload the current music and load the new music."
     * 
     * The function is a bit more complicated than that, but that's the gist of it
     * 
     * @param musicPath
     */
    void CMusic::setMusic(const std::string &musicPath) {
        std::cout << "musicPath: " << musicPath << std::endl;
        unload();
        if (!FileExists(musicPath.c_str()))
            throw std::runtime_error("Music file not found: " + musicPath);
        _music = std::make_shared<Music>(LoadMusicStream(musicPath.c_str()));
    }

    /**
     * Sets the looping state of the music.
     * 
     * @param loop Whether or not the music should loop.
     */
    void CMusic::setLoop(bool loop) { _music->looping = loop; }
}
