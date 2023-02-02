/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.cpp by thibb1
*/

#include "manager/ECSManager.hpp"

ECSManager::ECSManager() : _spriteFactory(nullptr) {}

void ECSManager::InitGame() {
    _spriteFactory = new Engine::Archetype<Engine::CSprite, Engine::CPosition, Engine::CMovement>();
    auto entity = _spriteFactory->CreateEntity(Engine::CSprite("assets/sprites/boss.png"));
    _spriteMap["ship"] = entity;
    _spriteFactory->AddComponent(entity, Engine::CPosition());
    _spriteFactory->AddComponent(entity, Engine::CMovement());
    // pointer to CPosition component
    auto position = std::make_shared<Engine::CPosition>(_spriteFactory->GetComponent<Engine::CPosition>(entity));
    _spriteFactory->GetComponent<Engine::CMovement>(entity).SetPosition(position);
    _spriteFactory->GetComponent<Engine::CMovement>(entity).SetSpeed(Vector3{10, 10, 10});
    _spriteFactory->GetComponent<Engine::CMovement>(entity).SetActive(true);
}

void ECSManager::UpdateGame() {
    // Draw cSprite
    auto movement = _spriteFactory->GetComponent<Engine::CMovement>(_spriteMap["ship"]);
    movement.Update();
    auto [cSprite, cPosition] = _spriteFactory->GetComponent<Engine::CSprite, Engine::CPosition>(_spriteMap["ship"]);
    cPosition.AddPosition(movement.GetSpeed());
    auto position = cPosition.GetPosition();
    std::cout << "ECSManager::UpdateGame: " << position.x << " " << position.y << std::endl;
    if (cSprite.GetTexture() == nullptr)
        return;
    DrawTexture(*cSprite.GetTexture(), position.x, position.y, WHITE);
}

void ECSManager::DestroyGame() { DELETE_PTR(_spriteFactory); }
