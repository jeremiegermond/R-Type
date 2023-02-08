/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CRotation.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    class CRotation : public ComponentBase {
      public:
        /**
         * @brief Construct a new CRotation object
         */
        CRotation();

        /**
         * @brief Destroy the CRotation object
         */
        ~CRotation() override;

        /**
         * @brief Get the rotation
         * @return Vector3
         */
        [[nodiscard]] Vector3 getRotation() const;

        /**
         * @brief Set the rotation
         * @param Vector3 rotation
         */
        void setRotation(const Vector3 &rotation);

      private:
        Vector3 _rotation;
    };
}
