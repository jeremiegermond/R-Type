/*
 * EPITECH PROJECT, 2023
 * rtype
 * File description:
 * GameBase.hpp by thibb1
 */

#pragma once

#include "engine/manager/ECSManagerBase.hpp"
#include "engine/utils/Defines.hpp"

namespace Engine {
    /*
     * @brief Base class for all games
     */
    class GameBase {
        public:
            /**
             * @brief Construct a new Game Base object
             */
            GameBase();

            /**
             * @brief destroy the Game Base object
             */
            virtual ~GameBase();

            /**
             * @brief init the game
             */
            void init();

            /**
             * @brief update the game
             */
            void update();

            /**
             * @brief destroy the game
             */
            void destroy();

        protected:
            /**
             * @brief Register all engine components
             */
            void registerComponents();

            /**
             * @brief init the game
             */
            virtual void initGame() = 0;

            /**
             * @brief update the game
             */
            virtual void updateGame() = 0;
            /**
             * @brief destroy the game
             */
            virtual void destroyGame() = 0;

            /**
             * @brief Register all game components
             */
            virtual void registerComponentsGame() = 0;
    };
}
