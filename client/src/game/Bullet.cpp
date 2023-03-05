/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Bullet.cpp
*/

#include "game/Game.hpp"

void Game::addBullet(Vector3 position, Vector3 velocity) {
    Engine::CObject bullet;
    bullet.setActive(true);
    Engine::CPosition bulletPosition;
    bulletPosition.setPosition(position);
    Engine::CVelocity bulletVelocity;
    bulletVelocity.setVelocity(velocity);
    bulletVelocity.setActive(true);
    CCollider bulletCollider;
    bulletCollider.setSize(.5);
    bulletCollider.setPosition(position);
    bulletCollider.setActive(true);

    pModel bulletModel = nullptr;
    if (_models.contains("missile")) {
        bulletModel = _models["missile"];
    }
    auto id = _objectArchetype.createEntity(bullet, bulletModel, bulletPosition, Engine::CRotation(), Engine::CScale(0.08),
                                               pAnimation(nullptr), bulletVelocity, CHealth(), bulletCollider, CParticleEmitter(), CText());
    _bullets.emplace_back(id);
    playSound("shoot");
}

void Game::updateBullets() {
    // loop through all bullets
    for (auto bullet = _bullets.begin(); bullet != _bullets.end();) {
        updateEntity(*bullet);
        auto [bulletCollider, position] = _objectArchetype.getComponent<CCollider, Engine::CPosition>(*bullet);
        auto bulletPosition = position.getPosition();
        if (bulletPosition.y > 10 || bulletPosition.y < -10 || bulletPosition.x > 10 || bulletPosition.x < -10) {
            bullet = _bullets.erase(bullet);
            continue;
        }
        bool hit = false;
        for (auto &enemy : _enemies) {
            auto enemyCollider = _objectArchetype.getComponent<CCollider>(enemy.second);
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
