/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Bullet.cpp
*/

#include "game/Game.hpp"

void Game::addBullet(Vector3 position, Vector3 velocity) {
    auto bullet = _pObjectArchetype.createEntity(Engine::CScale(0.08), Engine::CPosition(), Engine::CVelocity(), Engine::CObject(),
                                                 Engine::CRotation({0}), CCollider());
    auto [cObject, cPosition, cVelocity, cCollider] =
        _pObjectArchetype.getComponent<Engine::CObject, Engine::CPosition, Engine::CVelocity, CCollider>(bullet);
    cObject.setActive(true);
    cPosition.setPosition(position);
    cVelocity.setVelocity(velocity);
    cVelocity.setActive(true);
    cCollider.setSize(.5);
    cCollider.setPosition(position);
    cCollider.setActive(true);
    if (_models.contains("missile")) {
        _pObjectArchetype.addComponent(bullet, pModel(_models["missile"]));
    }
    _bullets.emplace_back(bullet);
    playSound("shoot");
}

void Game::updateBullets() {
    // loop through all bullets
    for (auto bullet = _bullets.begin(); bullet != _bullets.end();) {
        updateEntity(*bullet);
        auto [bulletCollider, position] = _pObjectArchetype.getComponent<CCollider, Engine::CPosition>(*bullet);
        auto bulletPosition = position.getPosition();
        if (bulletPosition.y > 10 || bulletPosition.y < -10 || bulletPosition.x > 10 || bulletPosition.x < -10) {
            bullet = _bullets.erase(bullet);
            continue;
        }
        bool hit = false;
        for (auto &enemy : _enemies) {
            auto enemyCollider = _pObjectArchetype.getComponent<CCollider>(enemy.second);
            if (bulletCollider.isColliding(enemyCollider)) {
                bullet = _bullets.erase(bullet);
                hit = true;
                break;
            }
        }
        if (hit)
            continue;
        bullet++;
    }
}
