/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp
*/

#pragma once

#include "engine/manager/ECSManagerBase.hpp"

typedef Engine::Archetype<Engine::CObject, Engine::CModel, Engine::CPosition, Engine::CRotation, Engine::CScale, Engine::CAnimation> ObjectArchetype;

class ECSManager : public Engine::ECSManagerBase {
  public:
    ECSManager() = default;

    void initFactories() override;
    void updateGame() override;
    void destroyGame() override;
};
