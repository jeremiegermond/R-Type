/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CPosition.cpp
*/

#include "engine/components/CPosition.hpp"

namespace Engine {

    CPosition::CPosition() : _position(Vector3()) {}

    CPosition::CPosition(const Vector3 &position) : _position(position) {}

    void CPosition::setPosition(const Vector3 &position) { _position = position; }

    Vector3 CPosition::getPosition() const { return _position; }

    void CPosition::addPosition(const Vector3 &position) {
        _position.x += position.x;
        _position.y += position.y;
        _position.z += position.z;
    }
}
