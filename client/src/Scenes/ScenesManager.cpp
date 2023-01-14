/*
** EPITECH PROJECT, 2023
** SceneManager.cpp
** File description:
** SceneManager.cpp [WIP]
*/

#include "SceneManager.hpp"
#include "Game.hpp"

namespace rtype {
    SceneManager::SceneManager(Game *pGame) {
        gameRef = pGame;
        CreateMenuScene();
        scenes.front()->StartScene();
    }

    void SceneManager::CreateMenuScene() {
        auto *menu = new Scene;
    }
}