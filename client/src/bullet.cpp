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

/**
 * It updates the bullet's position by adding the velocity to the position
 */
void Bullet::update() {
    auto velocity = Vector3Scale(_velocity, GetFrameTime());
    _position = Vector3Add(_position, velocity);
    _boundingBox = GetBoundingBoxAroundPoint(_position, _scale);
}

/**
 * If the bullet's bounding box is colliding with the bounding box of the enemy, return true
 * 
 * @param boundingBox The bounding box of the object you want to check collision with.
 * 
 * @return a boolean value.
 */
bool Bullet::isColliding(BoundingBox boundingBox) { return CheckCollisionBoxes(_boundingBox, boundingBox); }

/**
 * Draws the bullet model at the bullet's position, scale, and color.
 * 
 * @param model The model to draw.
 */
void Bullet::Draw(Model model) { DrawModel(model, _position, _scale, _color); }

/**
 * Get the position of the bullet.
 * 
 * @return A Vector3 object.
 */
Vector3 Bullet::GetPosition() const { return _position; }

/**
 * Get the index of the light that this bullet is using.
 * 
 * @return The index of the light that the bullet is attached to.
 */
int Bullet::GetLightIndex() const { return _lightIndex; }

/**
 * Get the size of the bullet.
 * 
 * @return The size of the bullet.
 */
float Bullet::getSize() const { return _scale; }

/**
 * Get the color of the bullet.
 * 
 * @return The color of the bullet.
 */
Color Bullet::getColor() const { return _color; }
