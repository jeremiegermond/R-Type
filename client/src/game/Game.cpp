/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr), _udpClient(nullptr) {}

void Game::initGame() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    _ecsManager = std::make_shared<ECSManager>();
    _ecsManager->init();
    loadAssets("assets/assets.json");
    loadEntities("assets/levels/level_01.json");
}

void Game::updateGame() {
    _ecsManager->update();

    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
    }
}

void Game::destroyGame() {
    _ecsManager->destroy();
    _ecsManager = nullptr;
}

void Game::initSocket(const std::string &ip, unsigned short port) { _udpClient = std::make_shared<UdpClient>(ip, port); }

void Game::loadEntities(const std::string &path) {
    if (path.empty())
        throw std::runtime_error("Invalid path to entities file");
    if (!FileExists(path.c_str()))
        throw std::runtime_error("Entities file not found");
    // json

    auto objectFactory = _ecsManager->getArchetype<ObjectArchetype>("Object");
}
