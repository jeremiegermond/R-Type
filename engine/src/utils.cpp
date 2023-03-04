/*
** EPITECH PROJECT, 2023
** Project
** File description:
** utils.cpp
*/

#include "engine/utils/Defines.hpp"

std::string floatToString(float number, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}

std::string getPositionString(Vector3 position) { return floatToString(position.x) + "," + floatToString(position.y); }
