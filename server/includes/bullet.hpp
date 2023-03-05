/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** bullet.hpp by thibb1
*/

#pragma once

#include "utils.hpp"
/**
 * @brief Class used to create bullets in the server
 * 
 */
class Bullet {
  private:
    Vector2 _position;
    Vector2 _velocity;
    BoundingBox _boundingBox{};
    float _scale;
    int _damage;
    int _playerId;

  public:
  /**
   * @brief Construct a new Bullet object
   * 
   * @param position 
   * @param velocity 
   * @param scale 
   * @param playerId 
   */
    Bullet(Vector2 position, Vector2 velocity, float scale, int playerId)
        : _position(position), _velocity(velocity), _scale(scale), _damage(1), _playerId(playerId) {
        _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
    }
    /**
     * @brief Destroy the Bullet object
     * 
     */
    ~Bullet() = default;
    /**
     * @brief update the bullet in real-time
     * 
     * @param dt 
     */
    void update(float dt) {
        auto velocity = Vector2Scale(_velocity, dt);
        _position = Vector2Add(_position, velocity);
        _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
    }
    /**
     * @brief check if the bullet is colliding
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool isColliding(BoundingBox other) { return CheckCollisionBoxes(_boundingBox, other); }
    /**
     * @brief check if the bullet is OOB (Out Of Bounds)
     * 
     * @return true 
     * @return false 
     */
    bool isOutOfBounds() const { return _position.x < -10 || _position.x > 10 || _position.y < -10 || _position.y > 10; }
    /**
     * @brief Get the Position of the bullet
     * 
     * @return Vector2 
     */
    Vector2 getPosition() const { return _position; }
    /**
     * @brief Get the Damage of the bullet
     * 
     * @return int 
     */
    int getDamage() const { return _damage; }
    /**
     * @brief Get the Player Id of the bullet
     * 
     * @return int 
     */
    int getPlayerId() const { return _playerId; }
};
