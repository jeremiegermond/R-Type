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
         * @param Vector3 velocity
         */
        explicit CVelocity(const Vector3 &velocity);

        /**
         * @brief Destroy the CVelocity object
         */
        ~CVelocity() override = default;

        /**
         * @brief Update the velocity
         */
        void doUpdate() override;

        /**
         * @brief Set the velocity
         * @param Vector3 velocity
         */
        void setVelocity(Vector3 velocity);

        /**
         * @brief Get the speed
         * @return Vector3
         */
        [[nodiscard]] Vector3 getVelocity() const;

      private:
        Vector3 _velocity;
        Vector3 _frameSpeed;
    };
}
