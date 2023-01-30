/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Messages.hpp by thibb1
*/

#pragma once

#include "engine/graphics/Raylib.hpp"

namespace Engine {

struct SetPositionMessage {
    explicit SetPositionMessage(const Vector3 &position) : _position(position) {}
    Vector3 _position;
};

struct GetPositionMessage {
    explicit GetPositionMessage(Vector3 &position) : _position(position) {}
    Vector3 &_position;
};
} // namespace Engine
