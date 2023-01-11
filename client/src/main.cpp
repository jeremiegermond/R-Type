/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
}

int main(int ac, char *av[]) {
    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
    CloseWindow();
    return 0;
}
