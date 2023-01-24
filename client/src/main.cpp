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
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <set>
#include <thread>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

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
    Vector3 position;
    Vector3 velocity;
    float scale;
    int lightIndex;
    std::shared_ptr<Light> light;
} Bullet;

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 rotationGoal;
    float scale;
    ModelAnimation *animations;
    unsigned int animationCount;
    int currentAnimation;
    int currentFrame;
    Model model;
    std::vector<Texture2D *> textures;
} MyObject;

typedef struct {
    Texture2D texture;
    Vector2 scale;
    int columns;
    int rows;
} MyTexture;

typedef struct {
    MyTexture *texture;
    Vector2 position;
    int totalFrames;
    int currentFrame;
    int columns;
    int rows;
    int width;
    int height;
    float rotation;
} Particle2D;

BoundingBox getBounds(Vector3 position, float scale) {
    Vector3 scaleVector = {scale, scale, scale};
    Vector3 min = Vector3Subtract(position, scaleVector);
    Vector3 max = Vector3Add(position, scaleVector);
    return {min, max};
}

std::mutex mutex;

// loadAssets
void loadAssets(std::unordered_map<std::string, MyObject> &objects, std::unordered_map<std::string, MyTexture> &textures,
                std::unordered_map<std::string, Shader> &shaders, std::atomic<bool> &loading, const std::string &assetsPath) {
    // open json file
    std::ifstream f(assetsPath);
    json data = json::parse(f);

    // Load objects
    // mutex.lock();
    std::cout << "Loading objects..." << std::endl;
    for (auto &object : data["models"]) {
        MyObject myObject;
        myObject.position = Vector3Zero();
        myObject.rotation = Vector3Zero();
        myObject.rotationGoal = Vector3Zero();
        myObject.scale = 1;
        std::string name = object["name"];
        std::string path = object["path"];
        myObject.model = LoadModel(path.c_str());
        if (object.contains("scale")) {
            myObject.scale = object["scale"];
        }
        if (object.contains("rotation") && object["rotation"].size() == 3) {
            myObject.rotation = {object["rotation"][0], object["rotation"][1], object["rotation"][2]};
            myObject.rotationGoal = myObject.rotation;
            myObject.model.transform = MatrixRotateXYZ(myObject.rotation);
        }
        if (object.contains("position") && object["position"].size() == 3) {
            myObject.position = {object["position"][0], object["position"][1], object["position"][2]};
        }
        myObject.animationCount = 0;
        if (object.contains("animation")) {
            std::string animationPath = object["animation"]["path"];
            myObject.animations = LoadModelAnimations(animationPath.c_str(), &myObject.animationCount);
        }
        if (object.contains("textures")) {
            for (auto &texture : object["textures"]) {
                std::string type = texture["type"];
                std::string pathTexture = texture["path"];
                int materialIndex = texture["materialIndex"];
                myObject.textures.push_back(new Texture2D(LoadTexture(pathTexture.c_str())));
                if (type == "diffuse") {
                    SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_DIFFUSE, *myObject.textures.back());
                } else if (type == "normal") {
                    SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_NORMAL, *myObject.textures.back());
                } else if (type == "metallic") {
                    SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_METALNESS, *myObject.textures.back());
                }
            }
        }
        objects[name] = myObject;
    }
    std::cout << "Loaded " << objects.size() << " objects" << std::endl;

    // Load sprites
    for (auto &sprite : data["sprites"]) {
        std::string name = sprite["name"];
        std::string path = sprite["path"];
        int columns = sprite["columns"];
        int rows = sprite["rows"];
        MyTexture myTexture;
        myTexture.texture = LoadTexture(path.c_str());
        myTexture.columns = columns;
        myTexture.rows = rows;
        myTexture.scale = {1, 1};
        if (sprite.find("scale") != sprite.end() && sprite["scale"].size() == 2) {
            myTexture.scale = {sprite["scale"][0], sprite["scale"][1]};
        }
        textures[name] = myTexture;
    }
    std::cout << "Loaded " << textures.size() << " sprites" << std::endl;

    // Load shaders
    for (auto &shader : data["shaders"]) {
        std::string name = shader["name"];
        // check vertex
        Shader s;
        std::string pathFragment = shader["path_fragment"];
        if (shader.contains("path_vertex")) {
            std::string pathVertex = shader["path_vertex"];
            std::cout << "Loading shader " << name << " with vertex shader " << pathVertex << " and fragment shader " << pathFragment << std::endl;
            mutex.lock();
            s = LoadShader(pathVertex.c_str(), pathFragment.c_str());
            mutex.unlock();
        } else {
            mutex.lock();
            s = LoadShader(nullptr, pathFragment.c_str());
            mutex.unlock();
        }
        if (shader.contains("locs")) {
            for (auto &loc : shader["locs"]) {
                std::string nameLoc = loc["name"];
                std::string from = loc["from"];
                if (nameLoc == "loc_matrix_model") {
                    s.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(s, from.c_str());
                } else if (nameLoc == "loc_vector_light") {
                    s.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(s, from.c_str());
                }
            }
        }
        shaders[name] = s;
    }
    std::cout << "Loaded " << shaders.size() << " shaders" << std::endl;
    loading = false;
    // mutex.unlock();
    //  Unload json file
    f.close();
}

int main(int ac, char *av[]) {
    int screenHeight = 450;
    int screenWidth = 800;
    std::atomic<bool> loading(true);

    InitWindow(screenWidth, screenHeight, "it rayworks !");
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    SetWindowMonitor(0);
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
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, MyObject> objects;
    std::unordered_map<std::string, MyTexture> textures;
    std::set<int> availableLights;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        availableLights.insert(i);
    }

    // Define the camera to look into our 3d world
    Camera camera;
    camera.position = {0, 0, 30};           // Camera position
    camera.target = Vector3Zero();          // Camera looking at point
    camera.up = {0, 1, 0};                  // Camera up vector (rotation towards target)
    camera.fovy = 20;                       // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera mode type
    // SetCameraMode(camera, CAMERA_ORBITAL);    // Set an orbital camera mode

    Model cube = LoadModelFromMesh(GenMeshCube(.1, .1, .1));

    // Ambient light level (some basic lighting)
    // int ambientLoc = GetShaderLocation(shader, "ambient");
    // float lightPos[4] = {.1, .1, .1, 1};
    // SetShaderValue(shader, ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    float speed = 0.1f;
    float tp = 7.22f;

    float bulletSize = 1;
    float bulletSpeed = .2;
    float shrinkSpeed = .1;

    float rotSpeed = .1;

    // Create lights
    std::vector<std::shared_ptr<Light>> lights;
    int newIndex = *availableLights.begin();

    // Thread for loading
    std::thread LoadingThread(&loadAssets, std::ref(objects), std::ref(textures), std::ref(shaders), std::ref(loading), "assets/assets.json");

    // temp
    float lightPos[4] = {.1, .1, .1, 1};
    int ambientLoc;

    while (!WindowShouldClose()) {
        if (loading) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Loading...", screenWidth / 2, screenHeight / 2, 24, WHITE);
            EndDrawing();
            continue;
        }

        // Create first light
        if (lights.empty()) {
            // Ambient light level (some basic lighting)
            ambientLoc = GetShaderLocation(shaders["lighting"], "ambient");
            SetShaderValue(shaders["lighting"], ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

            lights.emplace_back(new Light(newIndex, LIGHT_POINT, Vector3Zero(), Vector3Zero(), WHITE, shaders["lighting"], .2));
            availableLights.erase(newIndex);
            setModelShader(&objects["corridor"].model, &shaders["lighting"]);

            LoadingThread.join();
        }

        // LoadingThread.join();
        objects["spaceship1"].rotation = Vector3Lerp(objects["spaceship1"].rotation, objects["spaceship1"].rotationGoal, rotSpeed);
        objects["spaceship1"].model.transform = MatrixRotateXYZ(objects["spaceship1"].rotation);

        // positionCorridor.x -= speed;
        objects["corridor"].position.x -= speed;
        if (objects["corridor"].position.x < -tp)
            objects["corridor"].position.x = 0;
        if (IsKeyDown(KEY_LEFT) && objects["spaceship1"].position.x > -8) {
            objects["spaceship1"].position.x -= speed;
            objects["spaceship1"].rotationGoal.y = -1.5;
        }
        if (IsKeyDown(KEY_RIGHT) && objects["spaceship1"].position.x < 8) {
            objects["spaceship1"].position.x += speed;
            objects["spaceship1"].rotationGoal.y = 1.5;
        }
        if (IsKeyDown(KEY_UP) && objects["spaceship1"].position.y < 4.4) {
            objects["spaceship1"].position.y += speed;
            objects["spaceship1"].rotationGoal.x = -speed * 2;
        }
        if (IsKeyDown(KEY_DOWN) && objects["spaceship1"].position.y > -4.4) {
            objects["spaceship1"].position.y -= speed;
            objects["spaceship1"].rotationGoal.x = speed * 2;
        }
        // Reset up down tilt if up or down is not pressed
        if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
            objects["spaceship1"].rotationGoal.x = 0;
        }
        if (IsKeyPressed(KEY_P)) {
            std::cout << "Some debug infos:" << std::endl;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            int newLightIndex = *availableLights.begin();

            // random color GREEN RED or BLUE
            Color color = {GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
            lights.emplace_back(new Light(newLightIndex, LIGHT_POINT, (Vector3){-2, 4.5, 0}, Vector3Zero(), color, shaders["lighting"], .01));
            Bullet b = {objects["spaceship1"].position,
                        {objects["spaceship1"].rotation.y < 0 ? -bulletSpeed : bulletSpeed, -objects["spaceship1"].rotation.x * bulletSpeed, 0},
                        bulletSize,
                        newLightIndex,
                        lights.back()};
            availableLights.erase(newLightIndex);
            bullets.emplace_back(b);
            lights.back().get()->setPosition(objects["spaceship1"].position);
        }
        // Reset the scene
        if (IsKeyPressed(KEY_R)) {
            objects["spaceship1"].position = Vector3Zero();
            bullets.clear();
            particles.clear();
            objects["spaceship1"].rotation = {0, 1.5, 0};
            objects["spaceship1"].rotationGoal = objects["spaceship1"].rotation;
            objects["e1116"].currentFrame = 0;
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
        if (objects["e1116"].currentFrame + 1 < objects["e1116"].animations[0].frameCount) {
            objects["e1116"].currentFrame++;
            UpdateModelAnimation(objects["e1116"].model, objects["e1116"].animations[0], objects["e1116"].currentFrame);
        }

        BoundingBox enemyBounds = getBounds({1, 0, 0}, 1);
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->position = Vector3Add(it->position, it->velocity);
            BoundingBox bulletBounds = getBounds(it->position, .11); //{Vector3Subtract(it->position, x), Vector3Add(it->position, x)};
            it->light->setPosition(it->position);
            if (CheckCollisionBoxes(enemyBounds, bulletBounds)) {
                std::cout << "Collision" << std::endl;
                Particle2D boom;
                boom.texture = &textures["explosion"];
                boom.position.x = it->position.x;
                boom.position.y = it->position.y;
                boom.currentFrame = 0;
                boom.rows = textures["explosion"].rows;
                boom.columns = textures["explosion"].columns;
                boom.totalFrames = boom.rows * boom.columns;
                boom.rotation = (float)sin(GetTime() * 10) * 90;
                boom.width = textures["explosion"].texture.width / boom.columns;
                boom.height = textures["explosion"].texture.height / boom.rows;
                particles2D.emplace_back(boom);
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

        lights[0]->setPosition(Vector3Add(objects["spaceship1"].position, {0, 2, 10}));
        for (auto &light : lights) {
            light->UpdateLightValues(shaders["lighting"]);
        }

        // update the light shader with the camera view position
        SetShaderValue(shaders["lighting"], shaders["lighting"].locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shaders["normal"], shaders["normal"].locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Draw normal for outline shader
        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        // setModelShader(&objects["e1116"]->model, &shaders["normal"]);
        // setModelShader(&objects["spaceship1"]->model, &shaders["normal"]);
        // DrawModel(objects["spaceship1"]->model, objects["spaceship1"]->position, objects["spaceship1"]->scale, WHITE);
        // DrawModel(objects["e1116"]->model, objects["e1116"]->position, objects["e1116"]->scale, WHITE);
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(BLACK);
        BeginMode3D(camera);
        // setModelShader(&objects["e1116"]->model, &shaders["lighting"]);
        // setModelShader(&objects["spaceship1"]->model, &shaders["lighting"]);

        DrawBoundingBox(GetModelBoundingBox(objects["corridor"].model), RED);

        // Draw rotating corridor

        DrawModel(objects["corridor"].model,
                  {objects["corridor"].position.x + -tp * 2, objects["corridor"].position.y, objects["corridor"].position.z}, 1, WHITE);
        DrawModel(objects["corridor"].model, {objects["corridor"].position.x + -tp, objects["corridor"].position.y, objects["corridor"].position.z},
                  1, WHITE);
        DrawModel(objects["corridor"].model, objects["corridor"].position, 1, WHITE);
        DrawModel(objects["corridor"].model, {objects["corridor"].position.x + tp, objects["corridor"].position.y, objects["corridor"].position.z}, 1,
                  WHITE);
        DrawModel(objects["corridor"].model,
                  {objects["corridor"].position.x + tp * 2, objects["corridor"].position.y, objects["corridor"].position.z}, 1, WHITE);

        // Draw player
        DrawModel(objects["spaceship1"].model, objects["spaceship1"].position, objects["spaceship1"].scale, WHITE);

        // Draw enemy
        DrawModel(objects["e1116"].model, objects["e1116"].position, objects["e1116"].scale, WHITE);

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
        for (auto &light : lights) {
            DrawSphereWires(light->getPosition(), 0.2f, 8, 8, ColorAlpha(light->getColor(), 0.3f));
        }

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
                Rectangle source;
                source.x = (float)(it->width * (it->currentFrame % it->columns));
                source.y = (float)it->height * ((float)it->currentFrame / (float)it->rows);
                source.width = (float)it->texture->texture.width;
                source.height = (float)it->texture->texture.height;
                Vector3 position = {it->position.x, it->position.y, 3};
                Vector3 up = {0, 1, 0};
                Vector2 scale = it->texture->scale;
                DrawBillboardPro(camera, it->texture->texture, source, position, up, scale, Vector2Zero(), it->rotation, WHITE);
                ++it;
            }
        }
        EndMode3D();

        // BeginShaderMode(*shaders["outline"]);
        // DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
        //                (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(target.texture.height)}, Vector2Zero(), 0,
        //                WHITE);
        // EndShaderMode();
        DrawFPS(10, 10);
        DrawText("Use keys [LEFT][RIGHT][UP][DOWN][SPACE] to move and shoot", 10, 40, 20, DARKGRAY);
        EndDrawing();
    }
    UnloadModel(cube);
    UnloadRenderTexture(target);
    // unloads all the models
    for (auto &object : objects) {
        UnloadModel(object.second.model);
        for (auto &texture : object.second.textures) {
            UnloadTexture(*texture);
        }
        if (object.second.animationCount > 0)
            UnloadModelAnimations(object.second.animations, object.second.animationCount);
    }
    objects.clear();

    // unloads all the shaders
    for (auto &shader : shaders) {
        UnloadShader(shader.second);
    }
    shaders.clear();

    // unloads all the textures
    for (auto &texture : textures) {
        UnloadTexture(texture.second.texture);
    }
    textures.clear();

    CloseWindow();
    // LoadingThread.join();

    return 0;
}
