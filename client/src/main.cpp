/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#define RLIGHTS_IMPLEMENTATION
#include "light.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <set>
#include <thread>
#include <vector>

#define MAX_LIGHTS 100

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
    int spriteWidth;
    int spriteHeight;
    int columns;
    int rows;
    double rotation;
} Particle2D;

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float scale;
    int lightIndex;
    std::shared_ptr<Light> light;
} Bullet;

BoundingBox getBounds(Vector3 position, float scale) {
    Vector3 scaleVector = {scale, scale, scale};
    Vector3 min = Vector3Subtract(position, scaleVector);
    Vector3 max = Vector3Add(position, scaleVector);
    return {min, max};
}

int main(int ac, char *av[]) {
    int screenHeight = 450;
    int screenWidth = 800;
    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    SetWindowMonitor(1);
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
    std::set<int> availableLights;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        availableLights.insert(i);
    }

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
    Texture2D spriteBoss = LoadTexture("assets/sprites/boss.png");

    // Load models
    Model model = LoadModel("assets/models/lp_spaceship/spaceship.obj");
    Model enemy = LoadModel("assets/enemies.iqm");
    Texture2D enemyTexture = LoadTexture("assets/models/e116/enemies.png");
    Texture2D enemyNormal = LoadTexture("assets/models/e116/enemies_normal.png");
    Texture2D enemyMetallic = LoadTexture("assets/models/e116/enemies_metallic.png");
    SetMaterialTexture(&enemy.materials[0], MATERIAL_MAP_DIFFUSE, enemyTexture);
    SetMaterialTexture(&enemy.materials[0], MATERIAL_MAP_NORMAL, enemyNormal);
    SetMaterialTexture(&enemy.materials[0], MATERIAL_MAP_METALNESS, enemyMetallic);

    Model corridor = LoadModel("assets/models/corridor/corridor.obj");
    Model cube = LoadModelFromMesh(GenMeshCube(.1, .1, .1));

    Vector3 positionModel = Vector3Zero();
    Vector3 rotationModel = {0, 1.5, 0};
    Vector3 rotationEnemy = {-1.5, 0, 0};
    Vector3 positionCorridor = {0, 0, 0};
    Vector3 rotationCorridor = {0, 0, 0};

    model.transform = MatrixRotateZYX(rotationModel);
    enemy.transform = MatrixRotateZYX(rotationEnemy);
    corridor.transform = MatrixRotateZYX(rotationCorridor);
    setModelShader(&corridor, &shader);

    // animation
    unsigned int animsCount = 0;
    ModelAnimation *anims = LoadModelAnimations("assets/models/e116/enemies.iqm", &animsCount);
    int animFrame = 0;

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

    float bulletSize = 1;
    float bulletSpeed = .2;
    float shrinkSpeed = .1;

    Vector3 modelRotationGoal = rotationModel;
    float rotSpeed = .1;

    // Create lights
    std::vector<std::shared_ptr<Light>> lights;
    int newIndex = *availableLights.begin();
    lights.emplace_back(new Light(newIndex, LIGHT_POINT, Vector3Zero(), Vector3Zero(), WHITE, shader, .2));
    availableLights.erase(newIndex);

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

        if (IsKeyPressed(KEY_SPACE)) {
            int newLightIndex = *availableLights.begin();

            // random color GREEN RED or BLUE
            Color color = {GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
            lights.emplace_back(new Light(newLightIndex, LIGHT_POINT, (Vector3){-2, 4.5, 0}, Vector3Zero(), color, shader, .01));
            Bullet b = {positionModel,
                        {rotationModel.y < 0 ? -bulletSpeed : bulletSpeed, -rotationModel.x * bulletSpeed, 0},
                        bulletSize,
                        newLightIndex,
                        lights.back()};
            availableLights.erase(newLightIndex);
            bullets.emplace_back(b);
            lights.back().get()->setPosition(positionModel);
        }
        // Reset the scene
        if (IsKeyPressed(KEY_R)) {
            scale = 0.1f;
            positionModel = Vector3Zero();
            bullets.clear();
            particles.clear();
            rotationModel = {0, 1.5, 0};
            modelRotationGoal = rotationModel;
            animFrame = 0;
        }

        UpdateCamera(&camera);

        // Update bullets
        for (auto it = particles.begin(); it != particles.end();) {
            it->scale -= shrinkSpeed;
            if (it->scale <= 0) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }

        // Update animations
        if (animFrame + 1 < anims[0].frameCount) {
            animFrame++;
            UpdateModelAnimation(enemy, anims[0], animFrame);
        }

        BoundingBox enemyBounds = getBounds({1, 0, 0}, 1);
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->position = Vector3Add(it->position, it->velocity);
            BoundingBox bulletBounds = getBounds(it->position, .11); //{Vector3Subtract(it->position, x), Vector3Add(it->position, x)};
            it->light->setPosition(it->position);
            if (CheckCollisionBoxes(enemyBounds, bulletBounds)) {
                std::cout << "Collision" << std::endl;
                Particle2D boom = {spriteSheet,
                                   (Vector2){it->position.x, it->position.y},
                                   32,
                                   0,
                                   spriteSheet.width / 8,
                                   spriteSheet.height / 4,
                                   8,
                                   4,
                                   sin(GetTime() * 10) * 90};
                particles2D.emplace_back(boom);
                // particles2D.emplace_back(boss);
                it->light->setEnabled(false);
                it = bullets.erase(it);
            } else if (it->position.x > 10 || it->position.x < -10) {
                it->light->setEnabled(false);
                it = bullets.erase(it);
            } else {
                if (particles.size() >= 4500) {
                    it++;
                    continue;
                }
                Particle a = {it->position, bulletSize};
                particles.emplace_back(a);
                ++it;
            }
        }

        float cameraPos[3] = {camera.target.x, camera.target.y, camera.target.z};

        lights[0]->setPosition(Vector3Add(positionModel, {0, 2, 10}));
        for (auto &light : lights) {
            light->UpdateLightValues(shader);
        }

        // update the light shader with the camera view position
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(normShader, normShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Draw normal for outline shader
        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        setModelShader(&enemy, &normShader);
        setModelShader(&model, &normShader);
        DrawModel(model, positionModel, scale, WHITE);
        DrawModel(enemy, {1, 0, 0}, scale, WHITE);
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(GRAY);
        BeginMode3D(camera);
        setModelShader(&enemy, &shader);
        setModelShader(&model, &shader);
        DrawBoundingBox(GetModelBoundingBox(corridor), RED);

        // Draw rotating corridor
        DrawModel(corridor, {positionCorridor.x + -tp * 2, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + -tp, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, positionCorridor, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + tp, positionCorridor.y, positionCorridor.z}, 1, WHITE);
        DrawModel(corridor, {positionCorridor.x + tp * 2, positionCorridor.y, positionCorridor.z}, 1, WHITE);

        // Draw player
        DrawModel(model, positionModel, scale, WHITE);

        // Draw enemy
        DrawModel(enemy, {1, 0, 0}, scale, WHITE);

        // Draw bones enemy
        for (int i = 0; i < model.boneCount; i++) {
            DrawCube(anims[0].framePoses[animFrame][i].translation, 0.2f, 0.2f, 0.2f, RED);
        }

        DrawBoundingBox(enemyBounds, RED);
        lights.erase(std::remove_if(lights.begin(), lights.end(),
                                    [&availableLights](const std::shared_ptr<Light> &light) {
                                        if (!light->isEnabled()) {
                                            availableLights.insert(light->getIndex());
                                            return true;
                                        }
                                        return false;
                                    }),
                     lights.end());
        // Draw light sphere
        // for (auto &light : lights) {
        //    DrawSphereWires(light->getPosition(), 0.2f, 8, 8, ColorAlpha(light->getColor(), 0.3f));
        //}

        // Draw particles
        for (const auto &particle : particles) {
            DrawModel(cube, particle.position, particle.scale, WHITE);
        }

        // Draw bullets
        for (const auto &bullet : bullets) {
            DrawModel(cube, bullet.position, bulletSize, bullet.light->getColor());
        }

        // update 2d particles
        for (auto it = particles2D.begin(); it != particles2D.end();) {
            it->currentFrame += 1;
            if (it->currentFrame == it->totalFrames) {
                it = particles2D.erase(it);
            } else {
                auto x = (float)(it->spriteWidth * (it->currentFrame % it->columns));
                auto y = (float)(it->spriteHeight * (it->currentFrame / it->rows));
                DrawBillboardPro(camera, it->texture, (Rectangle){x, y, (float)it->spriteWidth, (float)it->spriteHeight},
                                 (Vector3){it->position.x, it->position.y, 3}, (Vector3){0, 1, 0}, {5, 5}, Vector2Zero(), it->rotation, WHITE);
                ++it;
            }
        }
        EndMode3D();

        BeginShaderMode(outline);
        DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(target.texture.height)}, Vector2Zero(), 0,
                       WHITE);
        EndShaderMode();
        DrawFPS(10, 10);
        DrawText("Use keys [LEFT][RIGHT][UP][DOWN][SPACE] to move and shoot", 10, 40, 20, DARKGRAY);
        EndDrawing();
    }
    UnloadShader(outline);
    UnloadShader(shader);
    UnloadShader(normShader);
    UnloadModel(model);
    UnloadModel(cube);
    UnloadModel(corridor);
    UnloadRenderTexture(target);
    UnloadTexture(spriteSheet);
    CloseWindow();
    return 0;
}
