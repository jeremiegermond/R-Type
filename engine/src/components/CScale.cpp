/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CScale.cpp
*/

#include "engine/components/CScale.hpp"

namespace Engine {
    /**
     * CScale::CScale() : _scale(0) {}
     */
    CScale::CScale() : _scale(0) {}

    /**
     * CScale::CScale(const float &scale) : _scale(scale) {}
     * 
     * @param scale The scale of the object.
     */
    CScale::CScale(const float &scale) : _scale(scale) {}

    CScale::~CScale() = default;

    /**
     * It returns the value of the private variable _scale.
     * 
     * @return The scale of the object.
     */
    float CScale::getScale() const { return _scale; }

    /**
     * The function sets the scale of the object
     * 
     * @param scale The scale of the object.
     */
    void CScale::setScale(const float &scale) { _scale = scale; }
}
