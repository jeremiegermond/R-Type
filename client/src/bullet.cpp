/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** bullet.cpp by thibb1
*/

#include "bullet.hpp"
#include "rtype-client.hpp"

Bullet::Bullet(Vector3 position, Vector3 velocity, float scale, int lightIndex, Color color)
    : _position(position), _velocity(velocity), _scale(scale), _lightIndex(lightIndex), _boundingBox(GetBoundingBoxAroundPoint(position, scale)),
      _color(color) {}

void Bullet::update() {
    auto velocity = Vector3Scale(_velocity, GetFrameTime());
    _position = Vector3Add(_position, velocity);
    _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
}

bool Bullet::isColliding(BoundingBox boundingBox) { return CheckCollisionBoxes(_boundingBox, boundingBox); }

void Bullet::Draw(Model model) { DrawModel(model, _position, _scale, _color); }

Vector3 Bullet::GetPosition() const { return _position; }

int Bullet::GetLightIndex() const { return _lightIndex; }

float Bullet::getSize() const { return _scale; }

Color Bullet::getColor() const { return _color; }
