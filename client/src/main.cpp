/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include "sparsemap.hpp"
#include <string>
#include <iostream>

/* int main() {
    InitWindow(1280, 720, "Window");

    EntityManager entities;
    MovementSystem movement(entities);

    int id = entities.create();
    entities.setPosition(id, 0.0f, 0.0f);
    entities.setVelocity(id, 1.0f, 2.0f);

    float x = 10, y = 10;

    while (!WindowShouldClose()) {
        movement.update(GetFrameTime());
        auto position = entities.getPosition(id);
        auto velocity = entities.getVelocity(id);
        x += velocity.x * GetFrameTime();
        y += velocity.y * GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("Position: (%f, %f)", position.x, position.y), x, y, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();

    return 0;
} */

int main() {
    ecs::SparseMap<std::string> sparsemap;

    for (int i = 0; i < 20; ++i) {
        sparsemap.insert(i, "block");
        std::cout << sparsemap[i] << std::endl;
        std::cout << i << std::endl;
    }
}