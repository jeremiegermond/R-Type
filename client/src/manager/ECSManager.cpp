/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.cpp by thibb1
*/

#include "manager/ECSManager.hpp"

void ECSManager::initGame() {
    //    spriteFactory = new Engine::Archetype<Engine::CSprite, Engine::CPosition, Engine::CVelocity>();
    auto spriteFactory = addArchetype<spriteArchetype>("spriteFactory");
    auto entity = spriteFactory->createEntity(Engine::CSprite("assets/sprites/explosion-sprite-sheet.png"));
    _spriteMap["ship"] = entity;
    spriteFactory->addComponent(entity, Engine::CPosition());
    spriteFactory->addComponent(entity, Engine::CVelocity());
    // pointer to CPosition component
    // spriteFactory->GetComponent<Engine::CVelocity>(entity).setPosition(spriteFactory->getComponent<Engine::CPosition
    spriteFactory->getComponent<Engine::CVelocity>(entity).setSpeed(Vector3{10, 10, 10});
    spriteFactory->getComponent<Engine::CVelocity>(entity).setActive(true);
}

void ECSManager::updateGame() {
    // Draw cSprite
    auto spriteFactory = getArchetype<spriteArchetype>("spriteFactory");
    // spriteFactory->getComponent<>()
    auto movement = spriteFactory->getComponent<Engine::CVelocity>(_spriteMap["ship"]);
    movement.update();
    auto [cSprite, cPosition] = spriteFactory->getComponent<Engine::CSprite, Engine::CPosition>(_spriteMap["ship"]);
    // cPosition.addPosition(movement.getSpeed());
    auto position = cPosition.getPosition();
    std::cout << "ECSManager::updateGame: " << position.x << " " << position.y << std::endl;
    if (cSprite.getTexture() == nullptr)
        return;
    DrawTexture(*cSprite.getTexture(), position.x, position.y, WHITE);
}

void ECSManager::destroyGame() {
    std::cout << "ECSManager::destroyGame" << std::endl;
    auto spriteFactory = getArchetype<spriteArchetype>("spriteFactory");
    spriteFactory->deleteComponent<Engine::CSprite>(_spriteMap["ship"]);
}
