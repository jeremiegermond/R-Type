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

namespace Engine {
using EntityId = uint32_t;
const EntityId NULL_ENTITY = std::numeric_limits<EntityId>::max();
} // namespace Engine
