/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.cpp
*/

#include "engine/components/CVelocity.hpp"

namespace Engine {

    CVelocity::CVelocity() : _velocity(Vector3Zero()), _frameSpeed(Vector3Zero()) {}

    CVelocity::CVelocity(const Vector3 &velocity) : _velocity(velocity), _frameSpeed(Vector3Zero()) {}

    void CVelocity::doUpdate() { _frameSpeed = Vector3Scale(_velocity, GetFrameTime()); }

    void CVelocity::setVelocity(Vector3 velocity) { _velocity = velocity; }

    Vector3 CVelocity::getVelocity() const { return _frameSpeed; }
}
