/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimatedSprite.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CAnimatedSprite : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CAnimatedSprite object
     * AnimatedSprite are used for textures.
     */
    CAnimatedSprite();

    /**
     * @brief Destroy the CAnimatedSprite object
     */
    ~CAnimatedSprite() override;

    /**
     * @brief Update the animated sprite
     */
    void doUpdate() override;

    /**
     * @brief Set the texture size
     * @param Vector2 size
     * @param int rows
     * @param int columns
     */
    void setTextureSize(Vector2 size, int rows, int columns);

    /**
     * @brief Get the source rectangle
     * @return Rectangle
     */
    [[nodiscard]] Rectangle getSourceRec() const;

    /**
     * @brief Get the scale
     * @return Vector2
     */
    [[nodiscard]] Vector2 getScale() const;

    /**
     * @brief Get the enabled
     * @return bool
     */
    [[nodiscard]] bool getEnabled() const;

    /**
     * @brief Set the rotation
     * @param float rotation
     */
    void setRotation(float rotation);

    /**
     * @brief Get the rotation
     * @return float
     */
    [[nodiscard]] float getRotation() const;

    /**
     * @brief Set the scale
     * @param Vector2 scale
     */
    void setScale(Vector2 scale);

  private:
    Vector2 _scale{};
    Rectangle _sourceRec;
    int _currentFrame;
    int _totalFrames;
    int _rows;
    int _columns;
    int _width;
    int _height;
    bool _enabled;
    float _rotation;
};
