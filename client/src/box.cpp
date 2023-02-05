/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** box.cpp by thibb1
*/

#include "rtype-client.hpp"

BoundingBox GetBoundingBoxAroundPoint(Vector3 position, float scale) {
    BoundingBox box = {position, position};
    Vector3 scaleVector = {scale, scale, scale};
    // scale box to the given scale
    box.min = Vector3Subtract(box.min, scaleVector);
    // translate box to given position
    box.max = Vector3Add(box.max, scaleVector);
    return box;
}
