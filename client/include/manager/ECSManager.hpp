/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp by thibb1
*/

#pragma once

#include "engine/manager/ECSManagerBase.hpp"

class ECSManager : public Engine::ECSManagerBase {
    public:
        ECSManager();

        void InitGame() override;
        void UpdateGame() override;
        void DestroyGame() override;

    private:
        Engine::Archetype<Engine::CSprite, Engine::CPosition, Engine::CMovement> *_spriteFactory;
        std::unordered_map<std::string, Engine::EntityId> _spriteMap;
};
