/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** player.hpp by thibb1
*/

#pragma once

#include "utils.hpp"

class Player {
  private:
    Vector2 _position;
    BoundingBox _bounds{};
    int _hp;
    bool _isAlive;
    int _id;

  public:
    Player() : _position({0, 0}), _hp(100), _isAlive(true), _id(0) { _bounds = GetBoundingBoxAroundPoint(_position, 0.5); }
    explicit Player(int id) : Player() { _id = id; }
    Vector2 getPosition();
    int getHp() const;
    int getId() const;
    BoundingBox getBounds() const;
    bool isAlive() const;
    void setPosition(Vector2 pos);
    void setHp(int hp);
    void setAlive(bool alive);
    bool move(const std::string &direction, const Vector2 &position);
};
