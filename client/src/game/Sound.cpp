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
