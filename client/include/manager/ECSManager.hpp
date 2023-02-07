/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp by thibb1
*/

#pragma once

#include "engine/manager/ECSManagerBase.hpp"

typedef Engine::Archetype<Engine::CObject, Engine::CSprite, Engine::CPosition, Engine::CVelocity> spriteArchetype;
// cameraArchetype
typedef Engine::Archetype<Engine::CObject, Engine::CPosition, Engine::CVelocity> cameraArchetype;

class ECSManager : public Engine::ECSManagerBase {
    public:
        ECSManager() = default;

        void initGame() override;
        void updateGame() override;
        void destroyGame() override;

    private:
        EntityMap _spriteMap;
        EntityMap _cameraMap;
};
