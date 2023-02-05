/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** enemy.hpp by thibb1
*/

#pragma once

#include "deps.hpp"
#include "game-object.hpp"

class Enemy {
  private:
    Vector3 _position;
    Vector3 _velocity;
    BoundingBox _boundingBox{};
    int _hp;
    GameObject *_gameObject;

  public:
    Enemy(Vector3 position, Vector3 velocity, int hp, GameObject *gameObject)
        : _position(position), _velocity(velocity), _hp(hp), _gameObject(gameObject) {
        updateBoundingBox();
    }

    ~Enemy() = default;

    void update() {
        auto velocity = Vector3Scale(_velocity, GetFrameTime());
        _position = Vector3Add(_position, velocity);
        updateBoundingBox();
    }

    void updateBoundingBox() {
        _boundingBox = _gameObject->getBaseBoundingBox();
        // translate bounding box to enemy position
        _boundingBox.min = Vector3Add(_boundingBox.min, _position);
        _boundingBox.max = Vector3Add(_boundingBox.max, _position);
    }

    void draw() {
        auto drawPosition = _position;
        drawPosition.x -= 100;
        _gameObject->draw(drawPosition);
    }

    int getHp() const { return _hp; }

    Vector3 getPosition() const { return _position; }

    Vector3 getVelocity() const { return _velocity; }

    BoundingBox getBounds() const { return _boundingBox; }

    void setPosition(Vector3 position) { _position = position; }

    void setVelocity(Vector3 velocity) { _velocity = velocity; }

    void setHp(int hp) { _hp = hp; }

    void takeDamage(int damage) { _hp -= damage; }

    [[nodiscard]] bool isDead() const { return _hp <= 0; }
};
