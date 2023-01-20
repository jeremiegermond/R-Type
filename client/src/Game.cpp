/*
** EPITECH PROJECT, 2023
** game.cpp
** File description:
** Game.cpp [WIP]
*/

#include "Game.hpp"

namespace rtype {
Game::Game() {
    width = 1280;
    height = 720;
    currentScene = 0;
}

Game::~Game() {}

void Game::createWindow() {
    InitWindow(width, height, "Wouuw !");
    SetTargetFPS(60);
    std::srand(time(nullptr));
    try {
        // scenes = SceneManager(this);
    } catch (...) {
        exit(84);
        // throw (CriticalError("Loading failed, aborting.", __FILE__, __LINE__));
    }
}

void Game::run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", (width / 2) - 225, height / 2, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
}

void Game::ChangeScene(int i) {
    GetScene()->UnloadScene();
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    currentScene = i;
    GetScene()->StartScene();
}

Scene *Game::GetScene() {}

SceneManager Game::GetSceneManager() { return scenes; }

void Game::CloseGame() {}
} // namespace rtype
