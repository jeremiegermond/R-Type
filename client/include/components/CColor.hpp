/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CColor.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CColor : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CColor object
     */
    CColor();

    /**
     * @brief Destroy the CColor object
     */
    ~CColor() override;

    /**
     * @brief Get the color of the component
     * @return The color of the component
     */
    [[nodiscard]] Color getColor() const;

    /**
     * @brief Set the color of the component
     * @param color The color to set
     */
    void setColor(Color color);

  private:
    Color _color;
};
