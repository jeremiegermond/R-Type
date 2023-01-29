/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "raylib.h"
#include "../network/include.hpp"
#include "../network/session.cpp"
#include "../network/UdpRequest.cpp"
#include "../network/UdpClient.cpp"
#include "../network/id_generator.cpp"

using namespace std::chrono_literals;

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(UdpClient& client, Vector3 *cubePosition, Camera3D camera) {
    float speed = 1.0f;
    if (IsKeyPressed(KEY_RIGHT)) {
        cubePosition->x += speed;
        client.request((std::string)"right:" + std::to_string(cubePosition->x) + ":" + std::to_string(cubePosition->y));
    }
    if (IsKeyPressed(KEY_LEFT)) {
        cubePosition->x -= speed;
        client.request((std::string)"left:" + std::to_string(cubePosition->x) + ":" + std::to_string(cubePosition->y));
    }
    if (IsKeyPressed(KEY_UP)) {
        cubePosition->y += speed;
        client.request((std::string)"up:" + std::to_string(cubePosition->x) + ":" + std::to_string(cubePosition->y));
    }
    if (IsKeyPressed(KEY_DOWN)) {
        cubePosition->y -= speed;
        client.request((std::string)"down:" + std::to_string(cubePosition->x) + ":" + std::to_string(cubePosition->y));
    }
    BeginDrawing();
    BeginMode3D(camera);
    ClearBackground(BLACK);
    for (auto it: client.get_players()) {
        Vector3 pos = {it->get_pos()[0], it->get_pos()[1], 0};
        DrawCube(pos, 1, 1, 1, BLUE);
    }
    DrawCube(*cubePosition, 1, 1, 1, RED);
    DrawGrid(10, 1.0f);
    EndMode3D();
    client.show_requests();
    //int cnt = 0;
    //DrawText("Pending calls :", 0, cnt, 15, LIGHTGRAY);
    /*for (auto it: client.pending) {
        cnt += 20;
        std::string str = std::to_string(it.first) + ":" + std::to_string(it.second->get_state());
        std::cout << str << std::endl;
        DrawText(str.c_str(), 0, cnt, 15, LIGHTGRAY);
    }*/
    EndDrawing();
}

void replace_player(Vector3& cubePosition, std::string x, std::string y) {
    cubePosition.x = std::stof(x);
    cubePosition.y = std::stof(y);
}

void process_requests(UdpClient& client, Vector3& cubePosition) {
    std::vector<UdpRequest*> requests = client.get_requests();
    for (auto it: requests) {
        if (it->get_state() == 1) {
            std::cout << "Request " << it->get_id() << " is done" << std::endl;
            std::cout << "Response: " << it->get_response() << std::endl;
            std::vector<std::string> response = split(it->get_response(), ':');
            if (response[1] == "KO")
                replace_player(cubePosition, response[2], response[3]);
            client.pop_request(it->get_id());
            delete it;
        }
    }
}

int main(int ac, char *av[]) {
    UdpClient client;
    int curframe = 0;
    InitWindow(screenWidth, screenHeight, "Quoi ? Feur.");
    SetTargetFPS(60);
    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 1.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;
    while (!WindowShouldClose()) {
        if (!client.alive()) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Connecting to server ...", screenWidth / 2 - 48, 200, 20, LIGHTGRAY);
            EndDrawing();
            if (client.disconnected() && client.connect("127.0.0.1", 12345) == false)
                break;
            EndDrawing();
            continue;
        }
        if (curframe != GetFrameTime()) {
            curframe = GetFrameTime();
        process_requests(client, cubePosition);
        }
        UpdateDrawFrame(client, &cubePosition, camera);
    }
    CloseWindow();
    return 0;
}