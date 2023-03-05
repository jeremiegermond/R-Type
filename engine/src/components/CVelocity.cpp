/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.cpp
*/

#include "engine/components/CVelocity.hpp"

namespace Engine {

    /**
     * CVelocity::CVelocity() : _velocity(Vector3Zero()), _frameSpeed(Vector3Zero()) {}
     */
    CVelocity::CVelocity() : _velocity(Vector3Zero()), _frameSpeed(Vector3Zero()) {}

    /**
     * CVelocity::CVelocity(const Vector3 &velocity) : _velocity(velocity), _frameSpeed(Vector3Zero())
     * {}
     * 
     * @param velocity The velocity of the object.
     */
    CVelocity::CVelocity(const Vector3 &velocity) : _velocity(velocity), _frameSpeed(Vector3Zero()) {}

    /**
     * _frameSpeed = Vector3Scale(_velocity, GetFrameTime());
     */
    void CVelocity::doUpdate() { _frameSpeed = Vector3Scale(_velocity, GetFrameTime()); }

    /**
     * This function sets the velocity of the object to the velocity passed in.
     * 
     * @param velocity The velocity of the object.
     */
    void CVelocity::setVelocity(Vector3 velocity) { _velocity = velocity; }

    /**
     * The function returns the velocity of the object in the current frame
     * 
     * @return A Vector3 object.
     */
    Vector3 CVelocity::getVelocity() const { return _frameSpeed; }
}
