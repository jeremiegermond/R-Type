/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** bullet.hpp by thibb1
*/

#pragma once

#include "utils.hpp"

class Bullet {
  private:
    Vector2 _position;
    Vector2 _velocity;
    BoundingBox _boundingBox{};
    float _scale;
    int _damage;
    int _playerId;

  public:
    Bullet(Vector2 position, Vector2 velocity, float scale, int playerId)
        : _position(position), _velocity(velocity), _scale(scale), _damage(1), _playerId(playerId) {
        _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
    }
    ~Bullet() = default;
    void update(float dt) {
        auto velocity = Vector2Scale(_velocity, dt);
        _position = Vector2Add(_position, velocity);
        _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
    }
    bool isColliding(BoundingBox other) { return CheckCollisionBoxes(_boundingBox, other); }
    bool isOutOfBounds() const { return _position.x < -10 || _position.x > 10 || _position.y < -10 || _position.y > 10; }
    Vector2 getPosition() const { return _position; }
    int getDamage() const { return _damage; }
    int getPlayerId() const { return _playerId; }
};
