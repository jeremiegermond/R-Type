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

        void initGame() override;
        void updateGame() override;
        void destroyGame() override;

        void registerComponentsGame() override;

    private:
        ECSManager *_ecsManager;
};
