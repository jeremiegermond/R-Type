/*
** EPITECH PROJECT, 2023
** Project
** File description:
** utils.cpp
*/

#include "engine/utils/Defines.hpp"

/**
 * Convert a float to a string with a specified number of decimal places
 * 
 * @param number The number to convert to a string.
 * @param precision The number of digits after the decimal point.
 * 
 * @return A string.
 */
std::string floatToString(float number, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}

/**
 * It converts a Vector3 to a string.
 * 
 * @param position The position of the object.
 * 
 * @return A string.
 */
std::string getPositionString(Vector3 position) { return floatToString(position.x) + "," + floatToString(position.y); }
