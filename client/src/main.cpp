/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include "archetype.hpp"
#include "components.hpp"
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
    ecs::Archetype<ecs::Position, ecs::Velocity> archetype;

    ecs::entity_type entity = archetype.create_entity(ecs::Position{0, 0}, ecs::Velocity{1, 2});
    ecs::entity_type entity2 = archetype.create_entity(ecs::Position{1, 2}, ecs::Velocity{3, 4});

    auto [position, velocity] = archetype.query<0, 1>(entity);
    std::cout << "Entity: " << entity << " position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Entity: " << entity << " position: " << velocity.x << ", " << velocity.y << std::endl;

    auto [position2, velocity2] = archetype.query<0, 1>(entity2);
    std::cout << "Entity: " << entity2 << " position: " << position2.x << ", " << position2.y << std::endl;
    std::cout << "Entity: " << entity2 << " position: " << velocity2.x << ", " << velocity2.y << std::endl;

    ecs::Position &position3 = archetype.query<0>(entity);
    std::cout << "Entity: " << entity << " position: " << position3.x << ", " << position3.y << std::endl;

    return 0;
}
