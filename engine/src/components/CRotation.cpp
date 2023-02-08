/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CRotation.cpp
*/

#include "engine/components/CRotation.hpp"

namespace Engine {
    CRotation::CRotation() : _rotation(Vector3Zero()) {}

    CRotation::~CRotation() = default;

    Vector3 CRotation::getRotation() const { return _rotation; }

    void CRotation::setRotation(const Vector3 &rotation) { _rotation = rotation; }
}
