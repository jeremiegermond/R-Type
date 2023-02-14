/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CRotation.cpp
*/

#include "engine/components/CRotation.hpp"

namespace Engine {
    CRotation::CRotation() : _rotation(Vector3Zero()), _rotationGoal(Vector3Zero()) {}

    CRotation::CRotation(const Vector3 &rotation) : _rotation(rotation), _rotationGoal(rotation) {}

    CRotation::~CRotation() = default;

    Vector3 CRotation::getRotation() const { return _rotation; }

    void CRotation::setRotation(const Vector3 &rotation) { _rotation = rotation; }

    Vector3 CRotation::getRotationGoal() const { return _rotationGoal; }

    void CRotation::setRotationGoal(const Vector3 &rotation) { _rotationGoal = rotation; }

    void CRotation::doUpdate() { _rotation = Vector3Lerp(_rotation, _rotationGoal, 0.1); }
}
