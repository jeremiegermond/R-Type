/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp by thibb1
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr) {}

void Game::initGame() {
    _ecsManager = new ECSManager();
    _ecsManager->init();
}

void Game::updateGame() { _ecsManager->update(); }

void Game::destroyGame() {
    _ecsManager->destroy();
    DELETE_PTR(_ecsManager);
}

void Game::registerComponentsGame() {}
