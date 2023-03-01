/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CBox.cpp
*/

#include "components/CBox.hpp"

CBox::CBox() = default;

CBox::~CBox() = default;

void CBox::setSize(Vector2 size) {
    _size = size;
    calculate();
}

void CBox::setScale(float scale) {
    _scale = scale;
    calculate();
}

void CBox::setPosition(Vector2 position) {
    _position = position;
    calculate();
}

void CBox::calculate() {
    auto position = _position;
    auto size = _size;
    size = Vector2Scale(size, _scale);
    position = Vector2Subtract(position, Vector2Scale(size, 0.5f));
    _box = {position.x, position.y, size.x, size.y};
}

Rectangle CBox::getBox() const { return _box; }
