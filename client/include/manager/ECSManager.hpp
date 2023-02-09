/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp
*/

#pragma once

#include "engine/manager/ECSManagerBase.hpp"

typedef std::shared_ptr<Engine::CModel> pModel;
typedef std::shared_ptr<Engine::CAnimation> pAnimation;
typedef Engine::Archetype<Engine::CObject, pModel, Engine::CPosition, Engine::CRotation, Engine::CScale, pAnimation> ObjectArchetype;

class ECSManager : public Engine::ECSManagerBase {
  public:
    ECSManager() = default;

    void initFactories() override;
    void updateGame() override;
    void destroyGame() override;
};
