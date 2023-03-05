/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CPosition.cpp
*/

#include "engine/components/CPosition.hpp"

namespace Engine {

    /**
     * CPosition::CPosition() : _position(Vector3()) {}
     */
    CPosition::CPosition() : _position(Vector3()) {}

    /**
     * CPosition::CPosition(const Vector3 &position) : _position(position) {}
     * 
     * @param position The position of the object.
     */
    CPosition::CPosition(const Vector3 &position) : _position(position) {}

    /**
     * Sets the position of the object to the given position.
     * 
     * @param position The position of the object.
     */
    void CPosition::setPosition(const Vector3 &position) { _position = position; }

    /**
     * This function returns the position of the object.
     * 
     * @return A copy of the _position member variable.
     */
    Vector3 CPosition::getPosition() const { return _position; }

    /**
     * This function adds the position of the object to the position of the object
     * 
     * @param position The position to add to the current position.
     */
    void CPosition::addPosition(const Vector3 &position) {
        _position.x += position.x;
        _position.y += position.y;
        _position.z += position.z;
    }
}
