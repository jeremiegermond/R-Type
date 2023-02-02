/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.hpp by thibb1
*/

#pragma once

#include "engine/graphics/Raylib.hpp"
#include "engine/utils/Defines.hpp"

namespace Engine {
    class ComponentBase {
        public:
            /**
             * @brief Base class for all components
             */
            ComponentBase();

            virtual ~ComponentBase();

            /**
             * @brief Initialize the component
             */
            void Init();

            /**
             * @brief Update the component
             */
            void Update();

            /**
             * @brief Destroy the component
             */
            void Destroy();

            /**
             * @brief Set the component current state
             * @param active
             */
            void SetActive(bool active);

            /**
             * @brief Get the component current state
             * @return bool
             */
            [[nodiscard]] bool IsActive() const;

        protected:
            bool _active;

            virtual void DoInit() {}
            virtual void DoUpdate() {}
            virtual void DoDestroy() {}
    };
}
