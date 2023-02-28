/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity.cpp
*/

#include "game/Game.hpp"

void Game::updateEntity(Engine::EntityId id) {
    // components must use tuple, or it will result in a copy of the component
    auto [cPosition, cVelocity, cRotation] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CVelocity, Engine::CRotation>(id);
    cRotation.update();
    cVelocity.update();
    cPosition.addPosition(cVelocity.getVelocity());
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
    DrawModel(model, Vector3Add(position, offset), scale, WHITE);
}

void Game::addBullet(Vector3 position, Vector3 velocity) {
    std::cout << "add bullet at: " << position.x << " " << position.y << " " << position.z << std::endl;
    std::cout << "velocity: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
    auto bullet = _pObjectArchetype->createEntity(Engine::CScale(0.1), Engine::CPosition(), Engine::CVelocity(), Engine::CObject(),
                                                  Engine::CRotation(Vector3Zero()));
    auto [cObject, cPosition, cVelocity] = _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CVelocity>(bullet);
    cObject.setActive(true);
    cPosition.setPosition(position);
    cVelocity.setVelocity(velocity);
    if (_models.contains("missile")) {
        _pObjectArchetype->addComponent(bullet, pModel(_models["missile"]));
    }
    _bullets.emplace_back(bullet);
}
