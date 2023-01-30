/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** particle2d.cpp by thibb1
*/

#include "rtype-client.hpp"

Particle2D::Particle2D(MyTexture *texture, Vector3 position, Vector2 scale, float rotation)
    : _texture(texture), _position(position), _up(Vector3{0, 1, 0}), _scale(scale), _rotation(rotation), _enabled(true) {
    if (texture == nullptr)
        return;
    _columns = texture->columns;
    _rows = texture->rows;
    _totalFrames = _columns * _rows;
    _currentFrame = 0;
    _width = texture->texture.width / _columns;
    _height = texture->texture.height / _rows;
    _source = Rectangle{0, 0, float(_width), float(_height)};
}

void Particle2D::Update() {
    _currentFrame++;
    if (_currentFrame >= _totalFrames)
        _enabled = false;
    _source.x = float((_currentFrame % _columns) * _width);
    _source.y = (_currentFrame / _rows) * _height;
}

void Particle2D::Draw(Camera3D camera) {
    if (!_enabled)
        return;
    DrawBillboardPro(camera, _texture->texture, _source, _position, _up, _scale, Vector2Zero(), _rotation, WHITE);
}

bool Particle2D::IsEnabled() const { return _enabled; }
