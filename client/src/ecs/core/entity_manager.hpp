/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include <iostream>
#include <vector>
#include <raylib.h>

#include "entities.hpp"

class EntityManager {
public:
    int create() {
        int id = positions.size();
        positions.emplace_back();
        velocities.emplace_back();
        return id;
    }

    void setPosition(int id, float x, float y) {
        positions[id] = { x, y };
    }

    void setVelocity(int id, float x, float y) {
        velocities[id] = { x, y };
    }

    Position &getPosition(int id) {
        return positions[id];
    }

    std::vector<Position> &getPositions() {
        return positions;
    }

    Velocity &getVelocity(int id) {
        return velocities[id];
    }

protected:
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
};

class MovementSystem {
public:
    MovementSystem(EntityManager &entities) : entities(entities) {}

    void update(float dt) {
        for (int i = 0; i < entities.getPositions().size(); i++) {
            auto &position = entities.getPosition(i);
            auto &velocity = entities.getVelocity(i);
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        }
    }

protected:
    EntityManager &entities;
};