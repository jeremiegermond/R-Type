/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CCollider.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CCollider : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CCollider object
     */
    CCollider();

    /**
     * @brief Destroy the CCollider object
     */
    ~CCollider() override;

    /**
     * @brief Check if the collision box is colliding with another
     * @param BoundingBox box
     * @return bool
     */
    [[nodiscard]] bool isColliding(BoundingBox box) const;

    /**
     * @brief Check if the collision box is colliding with another
     * @param CCollider collider
     * @return bool
     */
    [[nodiscard]] bool isColliding(const CCollider &collider) const;

    /**
     * @brief Set the collision box position
     * @param Vector3 position
     */
    void setPosition(Vector3 position);

    /**
     * @brief Set the collision box size
     * @param Vector3 size
     */
    void setSize(Vector3 size);

    /**
     * @brief Set the collision box size
     * @param float size
     */
    void setSize(float size);

    /**
     * @brief Get the collision box
     * @return BoundingBox
     */
    [[nodiscard]] BoundingBox getBox() const;

  private:
    BoundingBox _box;
    Vector3 _size;
    Vector3 _position;
};
