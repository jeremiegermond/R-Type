/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.hpp by thibb1
*/

#pragma once

#include "engine/game/GameBase.hpp"
#include "engine/utils/Defines.hpp"
#include "manager/ECSManager.hpp"

class Game : public Engine::GameBase {
  public:
    Game();

    void InitGame() override;
    void UpdateGame() override;
    void DestroyGame() override;

    void RegisterComponentsGame() override;

  private:
    ECSManager *_ecsManager;
};
