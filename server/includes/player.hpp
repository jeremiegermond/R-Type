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
    std::string _name;
    int _score;

  public:
    Player() : _position({0, 0}), _hp(3), _isAlive(true), _id(0), _name("") { _bounds = GetBoundingBoxAroundPoint(_position, 0.5); }
    explicit Player(int id) : Player() { _id = id; }
    Vector2 getPosition();
    int getHp() const;
    int getId() const;
    BoundingBox getBounds() const;
    [[nodiscard]] bool isAlive() const;
    void setPosition(Vector2 pos);
    void setHp(int hp);
    void takeDamage(int damage);
    void setAlive(bool alive);
    bool move(const std::string &direction, const Vector2 &position);

    void setName(const std::string &name);
    std::string getName();
    void setScore(int score);
    int getScore();
    void addScore(int score);
};
