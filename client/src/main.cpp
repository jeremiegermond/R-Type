/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(Vector2 *ballPosition) {
    float speed = 10.0f;
    if (IsKeyDown(KEY_RIGHT))
        ballPosition->x += speed;
    if (IsKeyDown(KEY_LEFT))
        ballPosition->x -= speed;
    if (IsKeyDown(KEY_UP))
        ballPosition->y -= speed;
    if (IsKeyDown(KEY_DOWN))
        ballPosition->y += speed;
    BeginDrawing();
    ClearBackground(BLACK);
    DrawGrid(10, 10.0f);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    DrawCircleV(*ballPosition, 50, MAROON);
    EndDrawing();
}

int main(int ac, char *av[]) {
    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetTargetFPS(60);
    Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};
    while (!WindowShouldClose()) {
        UpdateDrawFrame(&ballPosition);
    }
    CloseWindow();
    return 0;
}
