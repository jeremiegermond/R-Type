/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Bullet.cpp
*/

#include "game/Game.hpp"

void Game::addBullet(Vector3 position, Vector3 velocity) {
    auto bullet = _pObjectArchetype->createEntity(Engine::CScale(0.08), Engine::CPosition(), Engine::CVelocity(), Engine::CObject(),
                                                  Engine::CRotation(Vector3Zero()), CCollider());
    auto [cObject, cPosition, cVelocity, cCollider] =
        _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CVelocity, CCollider>(bullet);
    cObject.setActive(true);
    cPosition.setPosition(position);
    cVelocity.setVelocity(velocity);
    cVelocity.setActive(true);
    cCollider.setSize(.5);
    cCollider.setPosition(position);
    cCollider.setActive(true);
    if (_models.contains("missile")) {
        _pObjectArchetype->addComponent(bullet, pModel(_models["missile"]));
    }
    _bullets.emplace_back(bullet);
    playSound("shoot");
}

void Game::updateBullets() {
    for (auto it = _bullets.begin(); it != _bullets.end();) {
        updateEntity(*it);
        bool isColliding = false;
        Vector3 position;
        auto bulletCollider = _pObjectArchetype->getComponent<CCollider>(*it);
        for (auto &enemy : _enemies) {
            auto [cCollider, cHealth] = _pObjectArchetype->getComponent<CCollider, CHealth>(enemy.second);
            if (bulletCollider.isColliding(cCollider)) {
                cHealth.takeDamage(1);
                isColliding = true;
                position = _pObjectArchetype->getComponent<Engine::CPosition>(enemy.second).getPosition();
                break;
            }
        }
        if (isColliding) {
            it = _bullets.erase(it);
            playSound("enemy_bomb");
            addAnimatedSprite("explosion", position);
        } else {
            ++it;
        }
    }
}
