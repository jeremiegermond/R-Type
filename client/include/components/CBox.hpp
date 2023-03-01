/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CBox.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CBox : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CBox object
     * Box are used for UI elements
     */
    CBox();

    /**
     * @brief Destroy the CBox object
     */
    ~CBox() override;

    /**
     * @brief Set box size
     * @param Vector2 size
     */
    void setSize(Vector2 size);

    /**
     * @brief Set box scale
     * @param float scale
     */
    void setScale(float scale);

    /**
     * @brief Set box position
     * @param Vector2 position
     */
    void setPosition(Vector2 position);

    /**
     * @brief Calculate box
     */
    void calculate();

    /**
     * @brief Get the box
     * @return Rectangle
     */
    [[nodiscard]] Rectangle getBox() const;

  private:
    Vector2 _size{};
    Vector2 _position{};
    float _scale{};
    Rectangle _box{};
};
