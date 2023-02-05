/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** box.cpp by thibb1
*/

#include "rtype-client.hpp"

/**
 * It returns a bounding box that is centered around the given position and has a size of scale
 * 
 * @param position The position of the point.
 * @param scale The size of the box.
 * 
 * @return A BoundingBox struct.
 */
BoundingBox GetBoundingBoxAroundPoint(Vector3 position, float scale) {
    BoundingBox box = {position, position};
    Vector3 scaleVector = {scale, scale, scale};
    box.min = Vector3Subtract(box.min, scaleVector);
    box.max = Vector3Add(box.max, scaleVector);
    return box;
}
