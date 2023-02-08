/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CScale.cpp
*/

#include "engine/components/CScale.hpp"

namespace Engine {
    CScale::CScale() : _scale(Vector3One()) {}

    CScale::~CScale() = default;

    Vector3 CScale::getScale() const { return _scale; }

    void CScale::setScale(const Vector3 &scale) { _scale = scale; }
}
