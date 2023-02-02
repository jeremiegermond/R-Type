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

    void GameBase::Init() {
        InitWindow(800, 450, "R-Type");
        InitGame();
        RegisterComponents();
    }

    void GameBase::Update() {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            UpdateGame();
            EndDrawing();
        }
    }

    void GameBase::Destroy() {
        DestroyGame();
        CloseWindow();
    }

    void GameBase::RegisterComponents() { RegisterComponentsGame(); }
}
