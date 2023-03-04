/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CCollider.cpp
*/

#include "components/CCollider.hpp"

CCollider::CCollider() : _box(), _size(), _position() {}

CCollider::~CCollider() = default;

bool CCollider::isColliding(BoundingBox box) const { return CheckCollisionBoxes(_box, box); }

bool CCollider::isColliding(const CCollider &collider) const { return isColliding(collider.getBox()); }

void CCollider::setPosition(Vector3 position) {
    _position = position;
    _box.min = Vector3Subtract(position, _size);
    _box.max = Vector3Add(position, _size);
}

void CCollider::setSize(Vector3 size) {
    _size = Vector3Divide(size, {2, 2, 2});
    _box.min = Vector3Subtract(_position, size);
    _box.max = Vector3Add(_position, size);
}

void CCollider::setSize(float size) { setSize({size, size, size}); }

BoundingBox CCollider::getBox() const { return _box; }
