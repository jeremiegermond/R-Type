/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CRotation.cpp
*/

#include "engine/components/CRotation.hpp"

namespace Engine {
    /**
     * CRotation::CRotation() : _rotation(Vector3Zero()), _rotationGoal(Vector3Zero()) {}
     */
    CRotation::CRotation() : _rotation(Vector3Zero()), _rotationGoal(Vector3Zero()) {}

    /**
     * CRotation::CRotation(const Vector3 &rotation) : _rotation(rotation), _rotationGoal(rotation) {}
     * 
     * @param rotation The current rotation of the object.
     */
    CRotation::CRotation(const Vector3 &rotation) : _rotation(rotation), _rotationGoal(rotation) {}

    /**
     * CRotation::~CRotation() = default;
     */
    CRotation::~CRotation() = default;

    /**
     * This function returns the rotation of the object.
     * 
     * @return The rotation of the object.
     */
    Vector3 CRotation::getRotation() const { return _rotation; }

    /**
     * Sets the rotation of the object to the given rotation.
     * 
     * @param rotation The rotation to set the object to.
     */
    void CRotation::setRotation(const Vector3 &rotation) { _rotation = rotation; }

    /**
     * It returns the rotation goal.
     * 
     * @return The rotation goal.
     */
    Vector3 CRotation::getRotationGoal() const { return _rotationGoal; }

    /**
     * Sets the rotation goal to the given rotation.
     * 
     * @param rotation The current rotation of the object.
     */
    void CRotation::setRotationGoal(const Vector3 &rotation) { _rotationGoal = rotation; }

    /**
     * "If the rotation is not equal to the goal rotation, then set the rotation to the goal rotation."
     * 
     * The above function is called every frame
     */
    void CRotation::doUpdate() { _rotation = Vector3Lerp(_rotation, _rotationGoal, 0.1); }
}
