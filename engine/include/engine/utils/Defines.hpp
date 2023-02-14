/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Defines.hpp
*/

#pragma once

#include "engine/CPPIncludes.hpp"

#define DELETE_PTR(x)                                                                                                                                \
    if (x) {                                                                                                                                         \
        delete x;                                                                                                                                    \
        x = nullptr;                                                                                                                                 \
    }

#define UNUSED(x) (void)(x)

std::string floatToString(float number, int precision = 1);

/**
 * Converts a Vector3 to a string in the format of 'x,y'.
 *
 * @param position The position of the object.
 *
 * @return A string containing the x and y coordinates of the position vector.
 */
std::string getPositionString(Vector3 position);

namespace Engine {
    using EntityId = uint32_t;
    const EntityId NULL_ENTITY = std::numeric_limits<EntityId>::max();
}
