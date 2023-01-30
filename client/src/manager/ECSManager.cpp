/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.cpp by thibb1
*/

#include "manager/ECSManager.hpp"

ECSManager::ECSManager() : _spriteManager(nullptr) {}

void ECSManager::InitGame() {
    _spriteManager = new Engine::Archetype<Engine::CTransform, Engine::Sprite>();
    // auto transform = Engine::CTransform();
    auto entity = _spriteManager->CreateEntity(Engine::CTransform());
    // set transform position
    auto transform = _spriteManager->Query<Engine::CTransform>(entity);
    transform.Init();
    Vector3 position;
    position.x = 100;
    position.y = 100;
    position.z = 0;
    // Engine::SetPositionMessage message(position);
    ////transform.SendMessage(Engine::SetPositionMessage(position));
    // transform.SendMessage(message);
    //// get transform position
    // Vector3 position2;
    // transform.SendMessage(Engine::GetPositionMessage(position2));
    // std::cout << "position2.x = " << position2.x << std::endl;
    // std::cout << "position2.y = " << position2.y << std::endl;
    // std::cout << "position2.z = " << position2.z << std::endl;
    // transform.SetPosition(static_cast<Engine::SetPositionMessage &>(Engine::GetPositionMessage(position)));
}

void ECSManager::UpdateGame() {}

void ECSManager::DestroyGame() { DELETE_PTR(_spriteManager); }
