/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#define RLIGHTS_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"
// include for backface culling rlEnableBackfaceCulling();
#include "rlgl.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

void setModelShader(Model *m, Shader *s) {
    for (int i = 0; i < m->materialCount; i++) {
        m->materials[i].shader = *s;
    }
}

typedef struct {
    Vector3 position;
    unsigned char alpha;
    float scale;
} Particle;

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

    std::vector<Vector3> bullets;
    std::vector<Particle> particles;

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

    // Load models
    Model model = LoadModel("assets/models/lp_spaceship/spaceship.obj");
    Model corridor = LoadModel("assets/models/corridor/corridor.obj");
    Model sphere = LoadModelFromMesh(GenMeshSphere(1, 20, 20));

    Vector3 positionModel = Vector3Zero();
    Vector3 rotationModel = {0, 1.5, 0};
    Vector3 positionCorridor = {0, 0, 0};
    Vector3 rotationCorridor = {0, 0, 0};

    model.transform = MatrixRotateZYX(rotationModel);
    corridor.transform = MatrixRotateZYX(rotationCorridor);
    corridor.materials[0].shader = shader;

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float lightPos[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    SetShaderValue(shader, ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

    // Create lights
    Light lights[MAX_LIGHTS] = {0};
    lights[0] = CreateLight(LIGHT_POINT, {-2, 1, -2}, Vector3Zero(), YELLOW, shader);
    lights[1] = CreateLight(LIGHT_POINT, {2, 1, 2}, Vector3Zero(), RED, shader);
    lights[2] = CreateLight(LIGHT_POINT, {-2, 1, 2}, Vector3Zero(), GREEN, shader);
    lights[3] = CreateLight(LIGHT_POINT, {2, 1, -2}, Vector3Zero(), BLUE, shader);
    lights[4] = CreateLight(LIGHT_POINT, {0, 2, 6}, Vector3Zero(), WHITE, shader);

    lights[0].enabled = false;
    lights[1].enabled = false;
    lights[2].enabled = false;
    lights[3].enabled = false;
    lights[4].enabled = true;

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
    bool update = true;

    while (!WindowShouldClose()) {
        Vector3 tilt = rotationModel;

        positionCorridor.x -= speed;
        if (positionCorridor.x < -tp)
            positionCorridor.x = 0;
        if (IsKeyDown(KEY_LEFT) && positionModel.x > -8) {
            positionModel.x -= speed;
            tilt.y = -1.5;
        }
        if (IsKeyDown(KEY_RIGHT) && positionModel.x < 8) {
            positionModel.x += speed;
        }
        if (IsKeyDown(KEY_UP) && positionModel.y < 4.4) {
            tilt.x -= speed * 2;
            positionModel.y += speed;
        }
        if (IsKeyDown(KEY_DOWN) && positionModel.y > -4.4) {
            tilt.x += speed * 2;
            positionModel.y -= speed;
        }
        model.transform = MatrixRotateZYX(tilt);
        if (IsKeyPressed(KEY_V)) {
            bullets.emplace_back(positionModel);
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
        if (IsKeyPressed(KEY_L))
            update = !update;
        if (IsKeyPressed(KEY_SPACE)) {
            update = true;
            scale = 0.1f;
            positionModel = Vector3Zero();
            lights[0].enabled = false;
            lights[1].enabled = false;
            lights[2].enabled = false;
            lights[3].enabled = false;
            lights[4].enabled = true;
        }
        if (IsKeyPressed(KEY_PAGE_DOWN)) {
            tp -= 0.01f;
        }
        if (IsKeyPressed(KEY_PAGE_UP)) {
            tp += 0.01f;
        }

        UpdateCamera(&camera);

        // Update bullets
        for (auto it = particles.begin(); it != particles.end();) {
            it->scale -= .01;
            it->alpha -= 20;
            if (it->position.x > 10 || it->scale <= 0) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->x += .1;
            Particle a = {*it, 255, .1};
            particles.emplace_back(a);
            if (it->x > 10) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }

        float cameraPos[3] = {camera.target.x, camera.target.y, camera.target.z};

        // Check key inputs to enable/disable lights
        if (IsKeyPressed(KEY_Y)) {
            lights[0].enabled = !lights[0].enabled;
        }
        if (IsKeyPressed(KEY_R)) {
            lights[1].enabled = !lights[1].enabled;
        }
        if (IsKeyPressed(KEY_G)) {
            lights[2].enabled = !lights[2].enabled;
        }
        if (IsKeyPressed(KEY_B)) {
            lights[3].enabled = !lights[3].enabled;
        }
        if (IsKeyPressed(KEY_W)) {
            lights[4].enabled = !lights[4].enabled;
        }

        for (auto &light : lights) {
            UpdateLightValues(shader, light);
            UpdateLightValues(normShader, light);
        }

        // update the light shader with the camera view position
        if (update) {
            SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
            SetShaderValue(normShader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        }

        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        setModelShader(&model, &normShader);
        setModelShader(&sphere, &normShader);
        DrawModel(model, positionModel, scale, WHITE);

        for (auto it = particles.begin(); it != particles.end();) {
            DrawModel(sphere, it->position, it->scale, {255, 255, 255, it->alpha});
            ++it;
        }
        for (auto it = bullets.begin(); it != bullets.end();) {
            DrawModel(sphere, *it, 0.1, WHITE);
            ++it;
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
        DrawModel(model, positionModel, scale, WHITE);
        for (auto &light : lights) {
            if (light.enabled)
                DrawSphereEx(light.position, 0.2f, 8, 8, light.color);
            else
                DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(light.color, 0.3f));
        }

        for (auto it = particles.begin(); it != particles.end();) {
            DrawModel(sphere, it->position, it->scale, {255, 255, 255, it->alpha});
            ++it;
        }
        for (auto it = bullets.begin(); it != bullets.end();) {
            DrawModel(sphere, *it, 0.1, WHITE);
            ++it;
        }
        EndMode3D();

        BeginShaderMode(outline);
        DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(target.texture.height)}, Vector2Zero(), 0,
                       WHITE);
        EndShaderMode();
        // show the modified normals texture
        DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Rectangle){0, 0, static_cast<float>(target.texture.width / 4.0), static_cast<float>(target.texture.height / 4.0)},
                       Vector2Zero(), 0, WHITE);
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
    CloseWindow();
    return 0;
}
