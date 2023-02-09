/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.cpp
*/

#include "engine/components/CVelocity.hpp"

namespace Engine {

    CVelocity::CVelocity() : _speed(Vector3Zero()), _frameSpeed(Vector3Zero()) {}

    CVelocity::CVelocity(const Vector3 &speed) : _speed(speed), _frameSpeed(Vector3Zero()) {}

    void CVelocity::doUpdate() { _frameSpeed = Vector3Scale(_speed, GetFrameTime()); }

    void CVelocity::setSpeed(Vector3 speed) { _speed = speed; }

    Vector3 CVelocity::getSpeed() const { return _frameSpeed; }
}
