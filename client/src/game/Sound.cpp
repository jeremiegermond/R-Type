/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Sound.cpp
*/

#include "game/Game.hpp"

void Game::playSound(const std::string &name) {
    if (!_sounds.contains(name))
        return;
    auto sound = _sounds[name]->getSound();
    PlaySoundMulti(sound);
}

void Game::playMusic(const std::string &name) {
    if (!_musics.contains(name))
        return;
    if (_currentMusic == name)
        return;
    if (!_currentMusic.empty() && _musics.contains(_currentMusic)) {
        StopMusicStream(*_musics[_currentMusic]->getMusic());
    }
    auto music = _musics[name]->getMusic();
    SetMusicVolume(*music, 0.5f);
    PlayMusicStream(*music);
    _currentMusic = name;
}

void Game::updateMusic() {
    if (!_currentMusic.empty() && IsMusicStreamPlaying(*_musics[_currentMusic]->getMusic())) {
        UpdateMusicStream(*_musics[_currentMusic]->getMusic());
    }
}
