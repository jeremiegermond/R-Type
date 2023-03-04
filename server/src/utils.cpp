/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** regex.cpp by thibb1
*/

#include "utils.hpp"

/**
 * It returns true if the string str matches the regular expression pattern
 *
 * @param str The string to match against the pattern.
 * @param pattern The regular expression pattern to match against.
 *
 * @return A boolean value.
 */
bool isMatch(const std::string &str, const std::string &pattern) {
    std::regex reg(pattern);
    return std::regex_match(str, reg);
}

/**
 * If the string contains only digits, return true, otherwise return false.
 *
 * @param str The string to be tested.
 *
 * @return A boolean value.
 */
bool isNumber(const std::string &str) { return isMatch(str, "^[0-9]+$"); }

/**
 * It returns the nth match of a regular expression in a string
 *
 * @param str The string to search in
 * @param pattern The regular expression pattern to match.
 * @param index The index of the match to return.
 *
 * @return The string that matches the pattern.
 */
std::string getMatch(const std::string &str, const std::string &pattern, int index) {
    std::regex reg(pattern);
    std::smatch match;
    if (std::regex_search(str, match, reg)) {
        return match.str(index);
    }
    return "";
}

/**
 * It takes a string and a pattern, and returns a vector of strings that match the pattern
 *
 * @param str The string to search in.
 * @param pattern The regular expression pattern to match.
 *
 * @return A vector of strings.
 */
std::vector<std::string> getMatches(const std::string &str, const std::string &pattern) {
    std::regex reg(pattern);
    std::smatch match;
    std::vector<std::string> matches;
    if (std::regex_search(str, match, reg)) {
        for (auto &m : match) {
            matches.push_back(m.str());
        }
    }
    return matches;
}

/**
 * It converts a float to a string with a specified number of decimal places
 *
 * @param number The number to convert to a string.
 * @param precision The number of digits after the decimal point.
 *
 * @return A string
 */
std::string floatToString(float number, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}

/**
 * Converts a Vector2 to a string.
 *
 * @param vector2 The vector you want to convert to a string.
 *
 * @return A string
 */
std::string vectorToString(Vector2 vector2) { return floatToString(vector2.x) + "," + floatToString(vector2.y); }

/**
 * It takes a string and a delimiter character, and returns a vector of strings, each of which is a
 * substring of the original string, delimited by the delimiter character
 *
 * @param s The string to split
 * @param delim The delimiter character.
 *
 * @return A vector of strings
 */
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

/**
 * It returns a bounding box around a given point with a given scale
 *
 * @param vector2 The position of the point
 * @param scale The scale of the bounding box.
 *
 * @return A BoundingBox struct
 */
BoundingBox GetBoundingBoxAroundPoint(Vector2 vector2, float scale) {
    Vector3 position = {vector2.x, vector2.y, 0};
    BoundingBox box = {position, position};
    Vector3 scaleVector = {scale, scale, 0};
    // scale box to the given scale
    box.min = Vector3Subtract(box.min, scaleVector);
    // translate box to given position
    box.max = Vector3Add(box.max, scaleVector);
    return box;
}
