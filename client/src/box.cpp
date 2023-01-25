/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** box.cpp by thibb1
*/

#include "rtype-client.hpp"

BoundingBox GetMyObjectBoundingBox(const MyObject &myObject, Vector3 scale) {
    BoundingBox box = GetModelBoundingBox(myObject.model);
    // scale box to match model size
    box.min = Vector3Scale(box.min, myObject.scale);
    box.max = Vector3Scale(box.max, myObject.scale);
    // translate box to match model position
    box.min = Vector3Add(box.min, myObject.position);
    box.max = Vector3Add(box.max, myObject.position);
    // Add scale to box
    box.min = Vector3Subtract(box.min, scale);
    box.max = Vector3Add(box.max, scale);
    return box;
}

BoundingBox GetBoundingBoxAroundPoint(Vector3 position, float scale) {
    BoundingBox box = {position, position};
    Vector3 scaleVector = {scale, scale, scale};
    // scale box to the given scale
    box.min = Vector3Subtract(box.min, scaleVector);
    // translate box to given position
    box.max = Vector3Add(box.max, scaleVector);
    return box;
    // Better name for this function would be GetBoundingBoxAroundPoint
}
