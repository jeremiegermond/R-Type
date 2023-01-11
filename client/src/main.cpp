/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(Vector3 *cubePosition, Camera3D camera) {
    float speed = 1.0f;
    if (IsKeyPressed(KEY_RIGHT))
        cubePosition->x += speed;
    if (IsKeyPressed(KEY_LEFT))
        cubePosition->x -= speed;
    if (IsKeyPressed(KEY_UP))
        cubePosition->z -= speed;
    if (IsKeyPressed(KEY_DOWN))
        cubePosition->z += speed;
    BeginDrawing();
    BeginMode3D(camera);
    ClearBackground(BLACK);
    DrawCube(*cubePosition, 1, 1, 1, RED);
    DrawGrid(10, 1.0f);
    EndMode3D();
    EndDrawing();
}

int main(int ac, char *av[]) {
    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetTargetFPS(60);
    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 1.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;
    while (!WindowShouldClose()) {
        UpdateDrawFrame(&cubePosition, camera);
    }
    CloseWindow();
    return 0;
}
