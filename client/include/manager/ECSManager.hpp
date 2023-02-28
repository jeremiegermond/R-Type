/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp
*/

#pragma once

#include "components/CCamera.hpp"
#include "components/CCollider.hpp"
#include "components/CHealth.hpp"
#include "engine/manager/ECSManagerBase.hpp"

typedef std::shared_ptr<Engine::CModel> pModel;
typedef std::shared_ptr<Engine::CAnimation> pAnimation;
typedef Engine::Archetype<Engine::CObject, pModel, Engine::CPosition, Engine::CRotation, Engine::CScale, pAnimation, Engine::CVelocity, CHealth,
                          CCollider>
    ObjectArchetype;
typedef Engine::Archetype<Engine::CObject, CCamera> CameraArchetype;

class ECSManager : public Engine::ECSManagerBase {
  public:
    ECSManager() = default;

    void initFactories() override;
    void updateGame() override;
    void destroyGame() override;
};
