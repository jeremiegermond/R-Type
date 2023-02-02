/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CPosition.hpp by thibb1
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    class CPosition : public ComponentBase {
        public:
            CPosition();

            explicit CPosition(const Vector3 &position);

            /**
             * @brief Set the Position object
             * @param position
             */
            void SetPosition(const Vector3 &position);

            /**
             * @brief Get the Position object
             * @return Vector3
             */
            [[nodiscard]] Vector3 GetPosition() const;

            /**
             * @brief Add a Vector3 to the current position
             * @param position
             * @details This function will add the position to the current position
             */
            void AddPosition(const Vector3 &position);

        private:
            Vector3 _position;
    };
}
