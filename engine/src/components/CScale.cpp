/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CScale.cpp
*/

#include "engine/components/CScale.hpp"

namespace Engine {
    CScale::CScale() : _scale(0) {}

    CScale::CScale(const float &scale) : _scale(scale) {}

    CScale::~CScale() = default;

    float CScale::getScale() const { return _scale; }

    void CScale::setScale(const float &scale) { _scale = scale; }
}
