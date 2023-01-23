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
#include <raylib.h>

/* int main() {
    InitWindow(1280, 720, "Window");

    ecs::Archetype<ecs::Position, ecs::Velocity> archetype;

    ecs::entity_type entity = archetype.create_entity(ecs::Position{0, 0}, ecs::Velocity{1, 2});

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Salem", 0, 0, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();

    return 0;
} */

//int main() {
//    ecs::Archetype<ecs::Position, ecs::Velocity> archetype;
//
//    ecs::entity_type entity = archetype.create_entity(ecs::Position{0, 0}, ecs::Velocity{1, 2});
//    ecs::entity_type entity2 = archetype.create_entity(ecs::Position{1, 2}, ecs::Velocity{3, 4});
//
//    auto [position, velocity] = archetype.query<0, 1>(entity);
//    std::cout << "Entity: " << entity << " position: " << position.x << ", " << position.y << std::endl;
//    std::cout << "Entity: " << entity << " position: " << velocity.x << ", " << velocity.y << std::endl;
//
//    auto [position2, velocity2] = archetype.query<0, 1>(entity2);
//    std::cout << "Entity: " << entity2 << " position: " << position2.x << ", " << position2.y << std::endl;
//    std::cout << "Entity: " << entity2 << " position: " << velocity2.x << ", " << velocity2.y << std::endl;
//
//    ecs::Position &position3 = archetype.query<0>(entity);
//    std::cout << "Entity: " << entity << " position: " << position3.x << ", " << position3.y << std::endl;
//
//    return 0;
//}

int main() {
    ecs::SparseMap<int> sm;

    for (size_t i = 0; i < 35; ++i) {
        std::cout << "start: " << i << std::endl;
        sm.insert(i, i);
        std::cout << "after insert: " << i << std::endl;
        sm.erase(i);
        std::cout << "delete: " << i << std::endl;
    }
}