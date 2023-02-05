/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game.cpp by thibb1
*/

#include "rtype-client.hpp"

std::string floatToString(float number, int precision = 1) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << number;
    return out.str();
}

/**
 * Converts a Vector3 to a string in the format of 'x,y'.
 * 
 * @param position The position of the object.
 * 
 * @return A string containing the x and y coordinates of the position vector.
 */
std::string getPositionString(Vector3 position) { return floatToString(position.x) + "," + floatToString(position.y); }

/**
 * It moves the spaceship based on the keyboard input and sends the new position to the server
 * 
 * @param spaceship The spaceship object
 * @param client The client that will send the message to the server.
 */
void moveSpaceship(GameObject *spaceship, UdpClient *client) {
    float speed = 0.1;
    Vector3 position = spaceship->getPosition();
    Vector3 rotationGoal = spaceship->getRotationGoal();
    if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && position.x > -8) {
        position.x -= speed;
        client->send(std::string("move:left," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && position.x < 8) {
        position.x += speed;
        client->send(std::string("move:right," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN) && position.y < 4.2) {
        position.y += speed;
        rotationGoal.x = -.35;
        client->send(std::string("move:up," + getPositionString(position)));
    }
    if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP) && position.y > -4.2) {
        position.y -= speed;
        rotationGoal.x = .35;
        client->send(std::string("move:down," + getPositionString(position)));
    }
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        rotationGoal.x = 0;
    }
    spaceship->setPosition(position);
    spaceship->setRotationGoal(rotationGoal);
}
