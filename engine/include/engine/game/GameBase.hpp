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
             * @brief Destroy the Game Base object
             */
            virtual ~GameBase();

            /**
             * @brief Init the game
             */
            void Init();

            /**
             * @brief Update the game
             */
            void Update();

            /**
             * @brief Destroy the game
             */
            void Destroy();

        protected:
            /**
             * @brief Register all engine components
             */
            void RegisterComponents();

            /**
             * @brief Init the game
             */
            virtual void InitGame() = 0;

            /**
             * @brief Update the game
             */
            virtual void UpdateGame() = 0;
            /**
             * @brief Destroy the game
             */
            virtual void DestroyGame() = 0;

            /**
             * @brief Register all game components
             */
            virtual void RegisterComponentsGame() = 0;
    };
}
