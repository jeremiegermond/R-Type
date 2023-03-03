/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.hpp
*/

#pragma once

#include "components/CBox.hpp"
#include "components/CCamera.hpp"
#include "components/CCollider.hpp"
#include "components/CColor.hpp"
#include "components/CHandler.hpp"
#include "components/CHealth.hpp"
#include "components/CParticleEmitter.hpp"
#include "components/CText.hpp"
#include "components/CAnimatedSprite.hpp"

#include "engine/manager/ECSManagerBase.hpp"

typedef std::shared_ptr<Engine::CModel> pModel;
typedef std::shared_ptr<Engine::CAnimation> pAnimation;
typedef std::shared_ptr<Engine::CTexture> pTexture;

/**
 * The archetype of an object.
 * Objects are entities used for enemies, players, bullets, etc.
 */
typedef Engine::Archetype<Engine::CObject, pModel, Engine::CPosition, Engine::CRotation, Engine::CScale, pAnimation, Engine::CVelocity, CHealth,
                          CCollider, CParticleEmitter>
    ObjectArchetype;

/**
 * The archetype of a Sprite.
 * Sprite are entities used for textures.
 */

typedef Engine::Archetype<Engine::CPosition, Engine::CObject, CAnimatedSprite, pTexture> SpriteArchetype;

/**
 * The archetype of a camera.
 * Cameras are entities used to display the game.
 */
typedef Engine::Archetype<Engine::CObject, CCamera> CameraArchetype;

/**
 * The archetype of an UI element.
 * UI elements are entities used to display UI elements.
 */
typedef Engine::Archetype<Engine::CObject, CText, Engine::CPosition, Engine::CScale, CHandler, CBox, CColor> UIArchetype;

class ECSManager : public Engine::ECSManagerBase {
  public:
    ECSManager() = default;

    void initFactories() override;
    void updateGame() override;
    void destroyGame() override;
};
