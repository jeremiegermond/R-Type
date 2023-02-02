/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CPosition.cpp by thibb1
*/

#include "engine/components/CPosition.hpp"

namespace Engine {

    CPosition::CPosition() : _position(Vector3()) {}

    CPosition::CPosition(const Vector3 &position) : _position(position) {}

    void CPosition::SetPosition(const Vector3 &position) { _position = position; }

    Vector3 CPosition::GetPosition() const { return _position; }

    void CPosition::AddPosition(const Vector3 &position) {
        std::cout << "CPosition::AddPosition" << std::endl;
        _position.x += position.x;
        _position.y += position.y;
        _position.z += position.z;
    }
}
