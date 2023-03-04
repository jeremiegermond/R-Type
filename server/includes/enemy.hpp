/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** enemy.hpp by thibb1
*/

#pragma once

#include "utils.hpp"

class Enemy {
  private:
    int _id;
    Vector2 _position;
    Vector2 _velocity;
    BoundingBox _boundingBox{};
    int _hp;

  public:
    Enemy(int id, Vector2 position, Vector2 velocity, int hp) : _id(id), _position(position), _velocity(velocity), _hp(hp) {
        _boundingBox = GetBoundingBoxAroundPoint(_position, 0.5);
    }

    ~Enemy() = default;

    void update(float dt) {
        auto velocity = Vector2Scale(_velocity, dt);
        _position = Vector2Add(_position, velocity);
        _boundingBox = GetBoundingBoxAroundPoint(_position, 0.5);
    }

    int getId() const { return _id; }

    int getHp() const { return _hp; }

    Vector2 getPosition() const { return _position; }

    Vector2 getVelocity() const { return _velocity; }

    BoundingBox getBounds() const { return _boundingBox; }

    [[nodiscard]] bool isDead() const { return _hp <= 0; }

    void takeDamage(int damage) { _hp -= damage; }
};
