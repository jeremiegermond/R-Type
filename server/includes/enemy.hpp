/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** enemy.hpp by thibb1
*/

#pragma once

#include "utils.hpp"
/**
 * @brief Class used to create enemies in the server
 * 
 */
class Enemy {
  private:
    int _id;
    Vector2 _position;
    Vector2 _velocity;
    BoundingBox _boundingBox{};
    int _hp;

  public:
  /**
   * @brief Construct a new Enemy object
   * 
   * @param id 
   * @param position 
   * @param velocity 
   * @param hp 
   */
    Enemy(int id, Vector2 position, Vector2 velocity, int hp) : _id(id), _position(position), _velocity(velocity), _hp(hp) {
        _boundingBox = GetBoundingBoxAroundPoint(_position, 0.5);
    }
    /**
     * @brief Destroy the Enemy object
     * 
     */
    ~Enemy() = default;
    /**
     * @brief update the enemy
     * 
     * @param dt 
     */
    void update(float dt) {
        auto velocity = Vector2Scale(_velocity, dt);
        _position = Vector2Add(_position, velocity);
        _boundingBox = GetBoundingBoxAroundPoint(_position, 0.5);
    }
    /**
     * @brief return the Id 
     * 
     * @return int 
     */
    int getId() const { return _id; }
    /**
     * @brief return the Hp
     * 
     * @return int 
     */
    int getHp() const { return _hp; }
    /**
     * @brief Get the Position object
     * 
     * @return Vector2 
     */
    Vector2 getPosition() const { return _position; }
    /**
     * @brief Get the Velocity object
     * 
     * @return Vector2 
     */
    Vector2 getVelocity() const { return _velocity; }
    /**
     * @brief Get the Bounds object
     * 
     * @return BoundingBox 
     */
    BoundingBox getBounds() const { return _boundingBox; }
    /**
     * @brief check if enemy is dead
     * 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool isDead() const { return _hp <= 0; }
    /**
     * @brief reduce hp of enemy
     * 
     * @param damage 
     */
    void takeDamage(int damage) { _hp -= damage; }
};
