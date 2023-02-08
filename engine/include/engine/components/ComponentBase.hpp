/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.hpp
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
        void init();

        /**
         * @brief update the component
         */
        void update();

        /**
         * @brief destroy the component
         */
        void destroy();

        /**
         * @brief Set the component current state
         * @param active
         */
        void setActive(bool active);

        /**
         * @brief Get the component current state
         * @return bool
         */
        [[nodiscard]] bool isActive() const;

      protected:
        bool _active;

        virtual void doInit() {}
        virtual void doUpdate() {}
        virtual void doDestroy() {}
    };
}
