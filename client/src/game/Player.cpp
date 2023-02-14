/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Player.cpp
*/

#include "game/Game.hpp"

void Game::movePlayer() {
    float speed = 0.1;
    auto playerName = "R9A" + std::to_string(_playerId);
    auto playerEntity = _gameEntities[playerName];
    auto [cPosition, cRotation] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CRotation>(playerEntity);
    auto position = cPosition.getPosition();
    auto rotation = cRotation.getRotationGoal();
    if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && position.x > -8) {
        position.x -= speed;
        _udpClient->send(std::string("move:left," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && position.x < 8) {
        position.x += speed;
        _udpClient->send(std::string("move:right," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN) && position.y < 4.2) {
        position.y += speed;
        rotation.x = .35;
        _udpClient->send(std::string("move:up," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP) && position.y > -4.2) {
        position.y -= speed;
        rotation.x = -.35;
        _udpClient->send(std::string("move:down," + getPositionString(position)));
    }
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        rotation.x = 0;
    }
    cPosition.setPosition(position);
    cRotation.setRotationGoal(rotation);
}
