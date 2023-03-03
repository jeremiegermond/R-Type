/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity.cpp
*/

#include "game/Game.hpp"

void Game::updateEntity(Engine::EntityId id) {
    auto [cPosition, cVelocity, cRotation, cCollider] =
        _pObjectArchetype->getComponent<Engine::CPosition, Engine::CVelocity, Engine::CRotation, CCollider>(id);
    cRotation.update();
    cVelocity.update();
    cPosition.addPosition(cVelocity.getVelocity());
    cCollider.setPosition(cPosition.getPosition());
    auto [pModel, pAnimation] = _pObjectArchetype->getComponent<std::shared_ptr<Engine::CModel>, std::shared_ptr<Engine::CAnimation>>(id);
    if (pAnimation != nullptr) {
        pAnimation->update();
        auto animation = pAnimation->getAnimation();
        UpdateModelAnimation(pModel->getModel(), animation, pAnimation->getCurrentFrame());
    }
    auto [cObject, cEmitter] = _pObjectArchetype->getComponent<Engine::CObject, CParticleEmitter>(id);
    if (cObject.hasTag("emitter")) {
        cEmitter.setPosition(cPosition.getPosition());
        cEmitter.update();
    }
}

void Game::drawEntity(Engine::EntityId id, Vector3 offset) {
    auto cModel = _pObjectArchetype->getComponent<pModel>(id);
    if (cModel == nullptr)
        return;
    auto position = _pObjectArchetype->getComponent<Engine::CPosition>(id).getPosition();
    auto rotation = _pObjectArchetype->getComponent<Engine::CRotation>(id).getRotation();
    auto scale = _pObjectArchetype->getComponent<Engine::CScale>(id).getScale();
    auto model = cModel->getModel();
    model.transform = MatrixRotateXYZ(rotation);
    auto cCollider = _pObjectArchetype->getComponent<CCollider>(id);
    DrawBoundingBox(cCollider.getBox(), RED);
    auto cObject = _pObjectArchetype->getComponent<Engine::CObject>(id);
    if (cObject.hasTag("emitter")) {
        auto cEmitter = _pObjectArchetype->getComponent<CParticleEmitter>(id);
        cEmitter.draw();
    }
    DrawModel(model, Vector3Add(position, offset), scale, WHITE);
}

void Game::addEnemy(int id, Vector3 position, Vector3 velocity, int hp) {
    auto enemy = _enemies.contains(id) ? _enemies[id]
                                       : _pObjectArchetype->createEntity(Engine::CScale(0.1), Engine::CPosition(), Engine::CVelocity(),
                                                                         Engine::CObject(), Engine::CRotation({0, 3, 0}), CHealth(), CCollider());
    auto [cObject, cPosition, cVelocity, cHealth, cCollider] =
        _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CVelocity, CHealth, CCollider>(enemy);
    cObject.setActive(true);
    cPosition.setPosition(position);
    cVelocity.setVelocity(velocity);
    cVelocity.setActive(true);
    cHealth.setHealth(hp);
    cCollider.setActive(true);
    cCollider.setSize(1);
    cCollider.setPosition(position);
    if (_models.contains("E002")) {
        _pObjectArchetype->addComponent(enemy, pModel(_models["E002"]));
    }
    _enemies[id] = enemy;
}
