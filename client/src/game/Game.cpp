/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp by thibb1
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr) {}

void Game::InitGame() {
    _ecsManager = new ECSManager();
    _ecsManager->Init();
}

void Game::UpdateGame() { _ecsManager->Update(); }

void Game::DestroyGame() {
    _ecsManager->Destroy();
    DELETE_PTR(_ecsManager);
}

void Game::RegisterComponentsGame() {}
