/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.hpp
*/

#pragma once

#include "engine/components/CPosition.hpp"
#include "engine/components/ComponentBase.hpp"

namespace Engine {

    class CVelocity : public ComponentBase {
      public:
        /**
         * @brief Construct a new CVelocity object
         */
        CVelocity();

        /**
         * @brief Construct a new CVelocity object
         * @param Vector3 speed
         */
        explicit CVelocity(const Vector3 &speed);

        /**
         * @brief Destroy the CVelocity object
         */
        ~CVelocity() override = default;

        /**
         * @brief Update the velocity
         */
        void doUpdate() override;

        /**
         * @brief Set the speed
         * @param Vector3 speed
         */
        void setSpeed(Vector3 speed);

        /**
         * @brief Get the speed
         * @return Vector3
         */
        [[nodiscard]] Vector3 getSpeed() const;

      private:
        Vector3 _speed;
        Vector3 _frameSpeed;
    };
}
