/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Defines.hpp by thibb1
*/

#pragma once

#include "engine/CPPIncludes.hpp"

#define DELETE_PTR(x)                                                                                                                                \
    if (x) {                                                                                                                                         \
        delete x;                                                                                                                                    \
        x = nullptr;                                                                                                                                 \
    }

namespace Engine {
    using EntityId = uint32_t;
    const EntityId NULL_ENTITY = std::numeric_limits<EntityId>::max();
}
