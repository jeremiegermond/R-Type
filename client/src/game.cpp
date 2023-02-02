/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game.cpp by thibb1
*/

#include "rtype-client.hpp"

std::string getPositionString(Vector3 position) { return std::to_string(position.x) + ":" + std::to_string(position.y); }

void moveSpaceship(GameObject *spaceship, UdpClient *client) {
    float speed = 0.1;
    Vector3 position = spaceship->GetPosition();
    Vector3 rotationGoal = spaceship->GetRotationGoal();
    if (IsKeyDown(KEY_LEFT) && position.x > -8) {
        position.x -= speed;
        client->request("left:" + getPositionString(position));
    }
    if (IsKeyDown(KEY_RIGHT) && position.x < 8) {
        position.x += speed;
        client->request("right:" + getPositionString(position));
    }
    if (IsKeyDown(KEY_UP) && position.y < 4.4) {
        position.y += speed;
        rotationGoal.x = -.35;
        client->request("up:" + getPositionString(position));
    }
    if (IsKeyDown(KEY_DOWN) && position.y > -4.4) {
        position.y -= speed;
        rotationGoal.x = .35;
        client->request("down:" + getPositionString(position));
    }
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        rotationGoal.x = 0;
    }
    spaceship->SetPosition(position);
    spaceship->SetRotationGoal(rotationGoal);
}
