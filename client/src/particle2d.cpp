/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** particle2d.cpp by thibb1
*/

#include "rtype-client.hpp"

/**
 * It initializes the particle's texture, position, scale, rotation, and
 * enabled state
 * 
 * @param texture The texture to use for the particle.
 * @param position The position of the particle in 3D space.
 * @param scale The scale of the particle.
 * @param rotation The rotation of the particle in degrees.
 * 
 * @return A Particle2D object.
 */
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

/**
 * If the current frame is greater than or equal to the total frames, then the particle is disabled.
 * Otherwise, the source rectangle is updated to the next frame
 */
void Particle2D::Update() {
    _currentFrame++;
    if (_currentFrame >= _totalFrames)
        _enabled = false;
    _source.x = float((_currentFrame % _columns) * _width);
    _source.y = (_currentFrame / _rows) * _height;
}

/**
 * Draw a billboard with the given texture, source rectangle, position, up vector, scale, origin,
 * rotation and tint color
 * 
 * @param camera
 * 
 * @return A pointer to the Particle2D struct.
 */
void Particle2D::Draw(Camera3D camera) {
    if (!_enabled)
        return;
    DrawBillboardPro(camera, _texture->texture, _source, _position, _up, _scale, Vector2Zero(), _rotation, WHITE);
}

/**
 * It returns the value of the private variable _enabled.
 * 
 * @return A boolean value.
 */
bool Particle2D::IsEnabled() const { return _enabled; }
