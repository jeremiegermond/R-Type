/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CScale.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    class CScale : public ComponentBase {
      public:
        /**
         * @brief Construct a new CScale object
         */
        CScale();

        /**
         * @brief Destroy the CScale object
         */
        ~CScale() override;

        /**
         * @brief Get the scale
         * @return Vector3
         */
        [[nodiscard]] Vector3 getScale() const;

        /**
         * @brief Set the scale
         * @param Vector3 scale
         */
        void setScale(const Vector3 &scale);

      private:
        Vector3 _scale;
    };
}
