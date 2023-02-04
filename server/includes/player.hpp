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
    int _hp;
    bool _isAlive;
    int _id;

  public:
    Player() : _position({0, 0}), _hp(100), _isAlive(true), _id(0) {}
    explicit Player(int id) : _position({0, 0}), _hp(100), _isAlive(true), _id(id) {}
    Vector2 getPosition() { return _position; }
    int getHp() const { return _hp; }
    int getId() const { return _id; }
    bool isAlive() const { return _isAlive; }
    void setPosition(Vector2 pos) { _position = pos; }
    void setHp(int hp) { _hp = hp; }
    void setAlive(bool alive) { _isAlive = alive; }
    bool move(const std::string &direction, const Vector2 &position) {
        float speed = .1;
        if (direction == "right")
            _position.x += speed;
        else if (direction == "left")
            _position.x -= speed;
        else if (direction == "up")
            _position.y += speed;
        else if (direction == "down")
            _position.y -= speed;
        else
            return false;
        if (getPositionString(_position) != getPositionString(position))
            return false;
        return true;
    }
};
