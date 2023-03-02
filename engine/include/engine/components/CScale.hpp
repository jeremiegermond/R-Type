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
         * @brief Construct a new CScale object
         * @param float scale
         */
        explicit CScale(const float &scale);

        /**
         * @brief Destroy the CScale object
         */
        ~CScale() override;

        /**
         * @brief Get the scale
         * @return float
         */
        [[nodiscard]] float getScale() const;

        /**
         * @brief Set the scale
         * @param Vector3 scale
         */
        void setScale(const float &scale);

      private:
        float _scale;
    };
}
