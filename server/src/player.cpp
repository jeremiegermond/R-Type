/*
** EPITECH PROJECT, 2023
** B-CPP-500-LIL-5-2-rtype-romain.leemans
** File description:
** player
*/

#include "player.hpp"

/**
 * This function returns the position of the object.
 *
 * @return The position of the object.
 */
Vector2 Player::getPosition() { return _position; }
/**
 * GetHp() returns the value of the private member variable _hp.
 *
 * @return The value of the private variable _hp.
 */

int Player::getHp() const { return _hp; }
/**
 * GetId returns the value of the private member _id.
 *
 * @return The id of the player.
 */

int Player::getId() const { return _id; }
/**
 * It returns the bounding box of the player
 *
 * @return The bounding box of the player.
 */

BoundingBox Player::getBounds() const { return _bounds; }
/**
 * Returns true if the player is alive, false otherwise.
 *
 * @return A boolean value.
 */

bool Player::isAlive() const { return _isAlive; }
/**
 * Sets the position of the player to the given position.
 *
 * @param pos The position to set the player to.
 */

void Player::setPosition(Vector2 pos) { _position = pos; }
/**
 * This function sets the player's hp to the value passed in.
 *
 * @param hp The player's current health.
 */

void Player::setHp(int hp) { _hp = hp; }
/**
 * Sets the player's alive status to the value of the parameter.
 *
 * @param alive true if the player is alive, false if the player is dead
 */

void Player::setAlive(bool alive) { _isAlive = alive; }

/**
 * If the direction is valid, move the player in that direction and return true. Otherwise, return
 * false
 *
 * @param direction The direction the player is moving in.
 * @param position The position of the player
 *
 * @return A boolean value.
 */
bool Player::move(const std::string &direction, const Vector2 &position) {
    float speed = .1;
    if (direction == "right")
        _position.x += speed;
    else if (direction == "left")
        _position.x -= speed;
    else if (direction == "up")
        _position.y += speed;
    else if (direction == "down")
        _position.y -= speed;
    else
        return false;
    _bounds = GetBoundingBoxAroundPoint(_position, 0.5);
    if (vectorToString(_position) != vectorToString(position))
        return false;
    return true;
}

void Player::setName(const std::string &name) { _name = name; }

std::string Player::getName() { return _name; }
