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
