/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameBase.cpp by thibb1
*/

#include "engine/game/GameBase.hpp"

#include "raylib.h"

namespace Engine {

    GameBase::GameBase() = default;

    GameBase::~GameBase() = default;

    void GameBase::init() {
        InitWindow(800, 450, "R-Type");
        initGame();
        registerComponents();
    }

    void GameBase::update() {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            updateGame();
            EndDrawing();
        }
    }

    void GameBase::destroy() {
        destroyGame();
        CloseWindow();
    }

    void GameBase::registerComponents() { registerComponentsGame(); }
}
