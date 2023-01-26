/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game.cpp by thibb1
*/

#include "rtype-client.hpp"

void moveSpaceship(GameObject *&spaceship) {
    float speed = 0.1;
    Vector3 position = spaceship->GetPosition();
    Vector3 rotationGoal = spaceship->GetRotationGoal();
    if (IsKeyDown(KEY_LEFT) && position.x > -8) {
        position.x -= speed;
    }
    if (IsKeyDown(KEY_RIGHT) && position.x < 8) {
        position.x += speed;
    }
    if (IsKeyDown(KEY_UP) && position.y < 4.4) {
        position.y += speed;
        rotationGoal.x = -.35;
    }
    if (IsKeyDown(KEY_DOWN) && position.y > -4.4) {
        position.y -= speed;
        rotationGoal.x = .35;
    }
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        rotationGoal.x = 0;
    }
    spaceship->SetPosition(position);
    spaceship->SetRotationGoal(rotationGoal);
}
