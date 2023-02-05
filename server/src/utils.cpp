/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** regex.cpp by thibb1
*/

#include "utils.hpp"

bool isMatch(const std::string &str, const std::string &pattern) {
    std::regex reg(pattern);
    return std::regex_match(str, reg);
}

bool isNumber(const std::string &str) { return isMatch(str, "^[0-9]+$"); }

std::string getMatch(const std::string &str, const std::string &pattern, int index) {
    std::regex reg(pattern);
    std::smatch match;
    if (std::regex_search(str, match, reg)) {
        return match.str(index);
    }
    return "";
}

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

std::string floatToString(float number, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}

std::string vectorToString(Vector2 vector2) { return floatToString(vector2.x) + "," + floatToString(vector2.y); }

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

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
