/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.hpp by thibb1
*/

#pragma once

#include "engine/components/CTransform.hpp"
#include "engine/components/Sprite.hpp"
#include "engine/manager/Archetype.hpp"
#include "engine/utils/Defines.hpp"

namespace Engine {
class ECSManagerBase {
  public:
    ECSManagerBase();

    virtual ~ECSManagerBase();

    /**
     * @brief Init ECSManagerBase Archetypes
     */
    void Init();

    /**
     * @brief Update the ECSManagerBase
     */
    void Update();

    /**
     * @brief Destroy the ECSManagerBase
     */
    void Destroy();

  private:
    Archetype<float, int> *_testManager;

  protected:
    /**
     * @brief Init game Archetypes
     */
    virtual void InitGame() = 0;

    /**
     * @brief Update game Archetypes
     */
    virtual void UpdateGame() = 0;

    /**
     * @brief Destroy game Archetypes
     */
    virtual void DestroyGame() = 0;
};
} // namespace Engine
