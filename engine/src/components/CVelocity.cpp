/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.cpp
*/

#include <utility>

#include "engine/components/CVelocity.hpp"

namespace Engine {

CVelocity::CVelocity() : _speed(Vector3{0, 0, 0}), _position(nullptr) {}

void CVelocity::doUpdate() {
    if (!_position)
        return;
    std::cout << "CVelocity::doUpdate" << std::endl;
    Vector3 speed = getSpeed();
    _position->addPosition(speed);
}

void CVelocity::setSpeed(Vector3 speed) { _speed = speed; }

Vector3 CVelocity::getSpeed() const {
    float delta = GetFrameTime();
    Vector3 speed = Vector3Multiply(_speed, Vector3{delta, delta, delta});
    return speed;
}

void CVelocity::setPosition(std::shared_ptr<CPosition> position) {
    std::cout << "CVelocity::setPosition" << std::endl;
    _position = std::move(position);
    std::cout << "CVelocity::setPosition: " << _position << std::endl;
}
} // namespace Engine
