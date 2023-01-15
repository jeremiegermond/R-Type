/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#define RLIGHTS_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"
//#include "rlights.h"
#include "light.hpp"
#include "rlgl.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#define f static_cast<float>

void setModelShader(Model *m, Shader *s) {
    for (int i = 0; i < m->materialCount; i++) {
        m->materials[i].shader = *s;
    }
}

typedef struct {
    Vector3 position;
    float scale;
} Particle;

typedef struct {
    Texture2D texture;
    Vector2 position;
    int totalFrames;
    int currentFrame;
    int speed;
    int spriteWidth;
    int spriteHeight;
    int columns;
    int rows;
} Particle2D;

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float scale;
} Bullet;

int main(int ac, char *av[]) {
    int screenHeight = 450;
    int screenWidth = 800;
    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetWindowMonitor(2);
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    rlDisableBackfaceCulling();
    SetTargetFPS(60);

    int display = GetCurrentMonitor();
    screenHeight = GetMonitorHeight(display);
    screenWidth = GetMonitorWidth(display);
    SetWindowSize(screenWidth, screenHeight);
    ToggleFullscreen();

    std::vector<Bullet> bullets;
    std::vector<Particle> particles;
    std::vector<Particle2D> particles2D;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Loading...", screenWidth / 2, screenHeight / 2, 24, WHITE);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Define the camera to look into our 3d world
    Camera camera;
    camera.position = {0, 0, 30};           // Camera position
    camera.target = Vector3Zero();          // Camera looking at point
    camera.up = {0, 1, 0};                  // Camera up vector (rotation towards target)
    camera.fovy = 20;                       // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera mode type
    // SetCameraMode(camera, CAMERA_ORBITAL);    // Set an orbital camera mode

    // Load basic lighting shader
    Shader shader = LoadShader("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Sprite Sheet
    Texture2D spriteSheet = LoadTexture("assets/sprites/explosion-sprite-sheet.png");

    // Load models
    Model model = LoadModel("assets/models/lp_spaceship/spaceship.obj");
    Model black = LoadModel("assets/models/ehr_type/ertype_delme.obj");
    Model corridor = LoadModel("assets/models/corridor/corridor.obj");
    Model sphere = LoadModelFromMesh(GenMeshSphere(.1, 4, 4));

    Vector3 positionModel = Vector3Zero();
    Vector3 rotationModel = {0, 1.5, 0};
    Vector3 rotationBlack = {0, -1.5, 0};
    Vector3 positionCorridor = {0, 0, 0};
    Vector3 rotationCorridor = {0, 0, 0};

    model.transform = MatrixRotateZYX(rotationModel);
    black.transform = MatrixRotateZYX(rotationBlack);
    corridor.transform = MatrixRotateZYX(rotationCorridor);
    setModelShader(&corridor, &shader);
    setModelShader(&black, &shader);
    // black.materials[0].shader = shader;
    // corridor.materials[0].shader = shader;

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float lightPos[4] = {.1, .1, .1, 1};
    SetShaderValue(shader, ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

    // normal shader
    Shader normShader = LoadShader("assets/shaders/norm.vs", "assets/shaders/norm.fs");
    normShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(normShader, "matModel");

    // outline shader
    Shader outline = LoadShader(nullptr, "assets/shaders/outline.fs");

    std::cout << "\n\nScreen target is: " << screenWidth << "px width, " << screenHeight << "px height.\n" << std::endl;
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    float speed = 0.1f;
    float scale = 0.1f;
    float tp = 7.22f;

    float bullet_size = 1;
    float bullet_speed = .1;
    float shrink_speed = .02;

    Vector3 modelRotationGoal = rotationModel;
    float rotSpeed = .1;

    // Create lights
    std::vector<Light> lights;
    lights.emplace_back(0, LIGHT_POINT, (Vector3){-2, 4.5, 0}, Vector3Zero(), WHITE, shader, .1);
    lights.emplace_back(1, LIGHT_POINT, (Vector3){0, 4, 1}, Vector3Zero(), RED, shader, .05);
    lights[1].setEnabled(false);

    while (!WindowShouldClose()) {
        rotationModel = Vector3Lerp(rotationModel, modelRotationGoal, rotSpeed);
        model.transform = MatrixRotateZYX(rotationModel);

        positionCorridor.x -= speed;
        if (positionCorridor.x < -tp)
            positionCorridor.x = 0;
        if (IsKeyDown(KEY_LEFT) && positionModel.x > -8) {
            positionModel.x -= speed;
            modelRotationGoal.y = -1.5;
        }
        if (IsKeyDown(KEY_RIGHT) && positionModel.x < 8) {
            positionModel.x += speed;
            modelRotationGoal.y = 1.5;
        }
        if (IsKeyDown(KEY_UP) && positionModel.y < 4.4) {
            positionModel.y += speed;
            modelRotationGoal.x = -speed * 2;
        }
        if (IsKeyDown(KEY_DOWN) && positionModel.y > -4.4) {
            positionModel.y -= speed;
            modelRotationGoal.x = speed * 2;
        }
        // Reset up down tilt if up or down is not pressed
        if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
            modelRotationGoal.x = 0;
        }
        if (IsKeyPressed(KEY_V)) {
            Bullet b = {positionModel, {rotationModel.y < 0 ? -bullet_speed : bullet_speed, -rotationModel.x * bullet_speed, 0}, bullet_size};
            bullets.emplace_back(b);
            lights[1].setPosition(positionModel);
            lights[1].setEnabled(true);
        }
        if (IsKeyPressed(KEY_P)) {
            std::cout << "bullets:" << std::endl;
            std::cout << bullets.size() << std::endl;
            std::cout << "particles:" << std::endl;
            std::cout << particles.size() << std::endl;
            std::cout << "corridor:" << std::endl;
            std::cout << "corridor position:" << std::endl;
            std::cout << positionCorridor.x << std::endl;
            std::cout << positionCorridor.y << std::endl;
            std::cout << positionCorridor.z << std::endl;
            std::cout << "corridor rotation:" << std::endl;
            std::cout << rotationCorridor.x << std::endl;
            std::cout << rotationCorridor.y << std::endl;
            std::cout << rotationCorridor.z << std::endl;
            std::cout << "camera:" << std::endl;
            std::cout << "camera target:" << std::endl;
            std::cout << camera.target.x << std::endl;
            std::cout << camera.target.y << std::endl;
            std::cout << camera.target.z << std::endl;
            std::cout << "camera up:" << std::endl;
            std::cout << camera.up.x << std::endl;
            std::cout << camera.up.y << std::endl;
            std::cout << camera.up.z << std::endl;
            std::cout << "scale:" << std::endl;
            std::cout << scale << std::endl;
            std::cout << "tp:" << std::endl;
            std::cout << tp << std::endl;
            std::cout << "model:" << std::endl;
            std::cout << "model position:" << std::endl;
            std::cout << positionModel.x << std::endl;
            std::cout << positionModel.y << std::endl;
            std::cout << positionModel.z << std::endl;
        }

        UpdateCamera(&camera);

        // Update bullets
        for (auto it = particles.begin(); it != particles.end();) {
            it->scale -= shrink_speed;
            if (it->scale <= 0) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }

        Vector3 xx = {1, 1, 1};
        BoundingBox b = {Vector3Subtract({1, 0, 0}, xx), Vector3Add({1, 0, 0}, xx)};
        for (auto it = bullets.begin(); it != bullets.end();) {
            Vector3 x = {.11, .11, .11};
            it->position = Vector3Add(it->position, it->velocity);
            BoundingBox bbb = {Vector3Subtract(it->position, x), Vector3Add(it->position, x)};
            lights[1].setPosition(it->position);
            if (CheckCollisionBoxes(b, bbb)) {
                Particle2D boom = {
                    spriteSheet, (Vector2){it->position.x, it->position.y}, 32, 0, 30, spriteSheet.width / 8, spriteSheet.height / 4, 8, 4};
                particles2D.emplace_back(boom);
                it = bullets.erase(it);
                lights[1].setEnabled(false);
            } else if (it->position.x > 10 || it->position.x < -10) {
                it = bullets.erase(it);
                lights[1].setEnabled(false);
            } else {
                if (particles.size() >= 4500) {
                    it++;
                    continue;
                }
                Particle a = {it->position, 1};
                particles.emplace_back(a);
                ++it;
            }
        }

        float cameraPos[3] = {camera.target.x, camera.target.y, camera.target.z};

        if (IsKeyPressed(KEY_R)) {
            scale = 0.1f;
            positionModel = Vector3Zero();
            // Remove all bullets and particles
            bullets.clear();
            particles.clear();
            // Reset model rotation
            rotationModel = {0, 1.5, 0};
            modelRotationGoal = rotationModel;
        }
        lights[0].setPosition(Vector3Add(positionModel, {1, 1, 0}));
        for (auto &light : lights) {
            light.UpdateLightValues(shader);
        }

        // update the light shader with the camera view position
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(normShader, normShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        setModelShader(&model, &normShader);
        setModelShader(&sphere, &normShader);
        DrawModel(model, positionModel, scale, WHITE);

        for (auto &particle : particles) {
            DrawModel(sphere, particle.position, particle.scale, WHITE);
        }
        for (auto &bullet : bullets) {
            DrawModel(sphere, bullet.position, bullet.scale, WHITE);
        }
        EndMode3D();
        EndTextureMode();

        // Draw the scene
        ClearBackground(GRAY);
        BeginMode3D(camera);
        setModelShader(&sphere, &shader);
        setModelShader(&model, &shader);
        DrawBoundingBox(GetModelBoundingBox(corridor), RED);
        DrawModel(corridor, {positionCorridor.x + -tp * 2, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + -tp, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, positionCorridor, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + tp, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + tp * 2, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        // Draw player
        DrawModel(model, positionModel, scale, WHITE);
        // Draw black
        DrawModel(black, {1, 0, 0}, scale, WHITE);

        DrawBoundingBox(b, RED);
        for (auto &light : lights) {
            if (!light.isEnabled())
                DrawSphereEx(light.getPosition(), 0.2f, 8, 8, light.getColor());
            else
                DrawSphereWires(light.getPosition(), 0.2f, 8, 8, ColorAlpha(light.getColor(), 0.3f));
        }
        for (auto particle : bullets) {
            DrawModel(sphere, particle.position, particle.scale, WHITE);
        }
        for (auto bullet : bullets) {
            Vector3 x = {.11, .11, .11};
            BoundingBox bbb = {Vector3Subtract(bullet.position, x), Vector3Add(bullet.position, x)};
            DrawBoundingBox(bbb, RED);
            DrawModel(sphere, bullet.position, 1, WHITE);
            if (CheckCollisionBoxes(GetModelBoundingBox(model), bbb)) {
                std::cout << "Collision" << std::endl;
            }
        }
        // update 2d particles
        for (auto it = particles2D.begin(); it != particles2D.end();) {
            // it->currentFrame = (int)(GetTime() * it->speed) % it->totalFrames + 1;
            it->currentFrame += 1;
            if (it->currentFrame == it->totalFrames) {
                it = particles2D.erase(it);
            } else {

                auto x = (float)(it->spriteWidth * (it->currentFrame % it->columns));
                auto y = (float)(it->spriteHeight * (it->currentFrame / it->rows));
                std::cout << "it->position.x: " << it->position.x << " it->position.y: " << it->position.y << std::endl;

                // DrawTexturePro(it->texture, (Rectangle){x, y, (float)it->spriteWidth, (float)it->spriteHeight},
                //                (Rectangle){screenWidth/2 + it->position.x * 500 - it->spriteWidth/2 ,
                //                            screenHeight/2 + it->position.y * 500 - it->spriteHeight/2,
                //                            (float)it->spriteWidth, (float)it->spriteHeight},
                //                {0, 0}, 0, WHITE);
                DrawBillboardPro(camera, it->texture, (Rectangle){x, y, (float)it->spriteWidth, (float)it->spriteHeight},
                                 (Vector3){it->position.x, it->position.y, 3}, (Vector3){0, 1, 0}, {5, 5}, Vector2Zero(), 0, WHITE);
                ++it;
            }
        }
        EndMode3D();

        BeginShaderMode(outline);
        DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(target.texture.height)}, Vector2Zero(), 0,
                       WHITE);
        EndShaderMode();

        //// update 2d particles
        // for (auto it = particles2D.begin(); it != particles2D.end();) {
        //     //it->currentFrame = (int)(GetTime() * it->speed) % it->totalFrames + 1;
        //     it->currentFrame += 1;
        //     if (it->currentFrame == it->totalFrames) {
        //         it = particles2D.erase(it);
        //     } else {
        //
        //         auto x = (float)(it->spriteWidth * (it->currentFrame % it->columns));
        //         auto y = (float)(it->spriteHeight * (it->currentFrame / it->rows));
        //         std::cout << "it->position.x: " << it->position.x << " it->position.y: " << it->position.y << std::endl;
        //         DrawTexturePro(it->texture, (Rectangle){x, y, (float)it->spriteWidth, (float)it->spriteHeight},
        //                        (Rectangle){screenWidth/2 + it->position.x * 500 - it->spriteWidth/2 ,
        //                                    screenHeight/2 + it->position.y * 500 - it->spriteHeight/2,
        //                                    (float)it->spriteWidth, (float)it->spriteHeight},
        //                        {0, 0}, 0, WHITE);
        //         ++it;
        //     }
        // }
        float w = static_cast<float>(spriteSheet.width) / 8;
        float h = static_cast<float>(spriteSheet.height) / 4;
        // Get number from 0 to 7 with Getime
        auto n = (int)(GetTime() * 60) % 32;
        // 8 long 4 high
        // DrawTexturePro(spriteSheet, (Rectangle){w * (n % 8), h * (n / 8), h, w},
        //               (Rectangle){0, 0, w, h}, Vector2Zero(), 0, WHITE);
        //// show the modified normals texture
        // DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
        //                (Rectangle){0, 0, static_cast<float>(target.texture.width / 4.0), static_cast<float>(target.texture.height / 4.0)},
        //                Vector2Zero(), 0, WHITE);
        DrawFPS(10, 10);
        DrawText("Use keys [Y][R][G][B][W] to toggle lights", 10, 40, 20, DARKGRAY);
        EndDrawing();
    }
    UnloadShader(outline);
    UnloadShader(shader);
    UnloadShader(normShader);
    UnloadModel(model);
    UnloadModel(sphere);
    UnloadModel(corridor);
    UnloadRenderTexture(target);
    UnloadTexture(spriteSheet);
    CloseWindow();
    return 0;
}
