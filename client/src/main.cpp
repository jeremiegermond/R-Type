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

int main() {
    ecs::Archetype<ecs::Position, ecs::Velocity> archetype;
    ecs::entity_type entity = archetype.create_entity(ecs::Position{0, 0}, ecs::Velocity{1, 2});

    auto [position, velocity] = archetype.query<0, 1>(entity);
    archetype.delete_entity(entity);
    if (archetype.getDeletedEntities().count(entity) == 0) {
        std::cout << "Entity: " << entity << " position: " << position.x << ", " << position.y << std::endl;
        std::cout << "Entity: " << entity << " position: " << velocity.x << ", " << velocity.y << std::endl;
        std::cout << "Entity: " << entity << " position: " << positionb.x << ", " << positionb.y << std::endl;
        std::cout << "Entity: " << entity << " position: " << velocityb.x << ", " << velocityb.y << std::endl;
    } else {
        std::cout << "The entity "<< entity << " is deleted" << std::endl;
    }

    return 0;
}

//int main() {
//    ecs::SparseMap<int> sm;
//
//    sm.insert(5, 10);
//    sm.insert(8, 20);
//    sm.insert(1, 30);
//    sm.insert(3, 40);
//
//    std::cout << sm[5] << std::endl; // Outputs 10
//    std::cout << sm[8] << std::endl; // Outputs 20
//    std::cout << sm[1] << std::endl; // Outputs 30
//    std::cout << sm[3] << std::endl; // Outputs 40
//
//    sm.erase(5);
//    std::cout << sm[5] << std::endl; // Outputs 0
//
//    sm.insert(5, 20);
//    std::cout << sm[5] << std::endl; // Outputs 20
//
//    return 0;
//}