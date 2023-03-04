/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CColor.cpp
*/

#include "components/CColor.hpp"

CColor::CColor() : _color(WHITE) {}

CColor::~CColor() = default;

Color CColor::getColor() const { return _color; }

void CColor::setColor(Color color) { _color = color; }
