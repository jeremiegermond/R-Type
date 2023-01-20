/*
** EPITECH PROJECT, 2023
** SceneManager.cpp
** File description:
** SceneManager.cpp [WIP]
*/

#include "Game.hpp"
#include "SceneManager.hpp"

namespace rtype {
SceneManager::SceneManager(Game *pGame) {
    gameRef = pGame;
    CreateMenuScene();
    scenes.front()->StartScene();
}

void SceneManager::CreateMenuScene() { auto *menu = new Scene; }
} // namespace rtype
