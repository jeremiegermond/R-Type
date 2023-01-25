/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include "rtype-client.hpp"
#include "archetype.hpp"
#include "components.hpp"
#include <string>
#include <iostream>
#include <raylib.h>

void setModelShader(Model *m, Shader *s) {
    for (int i = 0; i < m->materialCount; i++) {
        m->materials[i].shader = *s;
    }
}

int main(int ac, char *av[]) {
    int screenHeight = 600;
    int screenWidth = 1070;

    InitWindow(screenWidth, screenHeight, "it rayworks !");
    InitAudioDevice();
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    MaximizeWindow();
    // SetWindowMonitor(0);
    rlDisableBackfaceCulling();
    SetTargetFPS(60);

    std::vector<Bullet> bullets;
    std::vector<Particle> particles;
    std::vector<Particle2D> particles2D;

    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, MyObject> objects;
    std::unordered_map<std::string, MyTexture> textures;
    std::unordered_map<std::string, MyMusic> musics;
    std::unordered_map<std::string, Sound> sounds;

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

    // Loading screen
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Loading...", screenWidth / 2, screenHeight / 2, 24, WHITE);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // load assets
    loadAssets(objects, musics, sounds, textures, shaders, "assets/assets.json");

    // Play menu music
    PlayMusicStream(musics["02-Main_Menu"].music);

    // ambient light
    float lightPos[4] = {.1, .1, .1, 1};
    int ambientLoc = GetShaderLocation(shaders["lighting"], "ambient");
    SetShaderValue(shaders["lighting"], ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

    lights.emplace_back(new Light(newIndex, LIGHT_POINT, Vector3Zero(), Vector3Zero(), WHITE, shaders["lighting"], .2));
    availableLights.erase(newIndex);
    setModelShader(&objects["corridor"].model, &shaders["lighting"]);
    setModelShader(&objects["rock_01"].model, &shaders["lighting"]);
    setModelShader(&objects["rock_02"].model, &shaders["lighting"]);
    setModelShader(&objects["space_debris_01"].model, &shaders["lighting"]);
    setModelShader(&objects["space_debris_02"].model, &shaders["lighting"]);
    setModelShader(&objects["space_debris_03"].model, &shaders["lighting"]);
    setModelShader(&objects["space_debris_04"].model, &shaders["lighting"]);
    setModelShader(&objects["space_debris_05"].model, &shaders["lighting"]);

    // Disable raylib debug messages
    SetTraceLogLevel(LOG_NONE);

    // temp
    BoundingBox box;
    Ray ray;
    Vector2 mouse;
    RayCollision collision;

    // selected myObject
    std::string selectedMyObjectName;
    MyObject *selectedMyObject = nullptr;
    // typedef struct {
    //    Rectangle bounds;
    //    float value;
    //    float minValue;
    //    float maxValue;
    //    bool dragging;
    //    bool enabled;
    //    Color baseColor;
    //    Color selectedColor;
    //    Color disabledColor;
    //} Slider;
    // Create sliders to move selected object
    Slider sliders[3];

    while (!WindowShouldClose()) {
        objects["spaceship1"].rotation = Vector3Lerp(objects["spaceship1"].rotation, objects["spaceship1"].rotationGoal, rotSpeed);
        objects["spaceship1"].model.transform = MatrixRotateXYZ(objects["spaceship1"].rotation);

        box = GetMyObjectBoundingBox(objects["spaceship1"]);
        objects["corridor"].position.x -= speed;
        if (objects["corridor"].position.x < -tp)
            objects["corridor"].position.x = 0;

        // Check if spaceship1 is in view
        // Matrix cameraMatrix = GetCameraMatrix(camera);
        Vector2 screenPos = GetWorldToScreen(objects["spaceship1"].position, camera);
        // add a margin of 10 pixels
        if (screenPos.x < -10 || screenPos.x > screenWidth + 10 || screenPos.y < -10 || screenPos.y > screenHeight + 10) {
            std::cout << "out of view" << std::endl;
        }

        // Left click to raycast on the scene
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mouse = GetMousePosition();
            ray = GetMouseRay(mouse, camera);
            // cycle through all objects of the scene
            for (auto &object : objects) {
                // check if the ray hits the object
                BoundingBox objectBox = GetMyObjectBoundingBox(object.second);
                collision = GetRayCollisionBox(ray, objectBox);
                if (collision.hit) {
                    std::cout << "hit " << object.first << std::endl;
                    selectedMyObjectName = object.first;
                    selectedMyObject = &object.second;
                    break;
                }
            }
            if (!collision.hit) {
                selectedMyObjectName = "";
                selectedMyObject = nullptr;
            }
        }

        if (IsKeyDown(KEY_LEFT) && objects["spaceship1"].position.x > -8) {
            objects["spaceship1"].position.x -= speed;
            objects["spaceship1"].rotationGoal.y = -1.5;
        }
        if (IsKeyDown(KEY_RIGHT)) { //&& objects["spaceship1"].position.x < 8) {
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

            unsigned char r = GetRandomValue(0, 255);
            unsigned char g = GetRandomValue(0, 255);
            unsigned char b = GetRandomValue(0, 255);
            Color color = {r, g, b, 255};
            lights.emplace_back(new Light(newLightIndex, LIGHT_POINT, (Vector3){-2, 4.5, 0}, Vector3Zero(), color, shaders["lighting"], .01));
            Bullet bullet = {objects["spaceship1"].position,
                             {objects["spaceship1"].rotation.y < 0 ? -bulletSpeed : bulletSpeed, -objects["spaceship1"].rotation.x * bulletSpeed, 0},
                             bulletSize,
                             newLightIndex,
                             lights.back()};
            availableLights.erase(newLightIndex);
            bullets.emplace_back(bullet);
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

        // Update camera
        UpdateCamera(&camera);

        // Update music
        UpdateMusicStream(musics["02-Main_Menu"].music);

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

        BoundingBox enemyBounds = GetMyObjectBoundingBox(objects["e1116"], {0, .5, 0});
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->position = Vector3Add(it->position, it->velocity);
            BoundingBox bulletBounds =
                GetBoundingBoxAroundPoint(it->position, .11); //{Vector3Subtract(it->position, x), Vector3Add(it->position, x)};
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
        setModelShader(&objects["e1116"].model, &shaders["normal"]);
        setModelShader(&objects["spaceship1"].model, &shaders["normal"]);
        DrawModel(objects["spaceship1"].model, objects["spaceship1"].position, objects["spaceship1"].scale, WHITE);
        DrawModel(objects["e1116"].model, objects["e1116"].position, objects["e1116"].scale, WHITE);
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(BLACK);
        BeginMode3D(camera);
        setModelShader(&objects["e1116"].model, &shaders["lighting"]);
        setModelShader(&objects["spaceship1"].model, &shaders["lighting"]);

        DrawModel(objects["rock_01"].model, objects["rock_01"].position, objects["rock_01"].scale, WHITE);
        DrawModel(objects["rock_02"].model, objects["rock_02"].position, objects["rock_02"].scale, WHITE);
        DrawModel(objects["space_debris_01"].model, objects["space_debris_01"].position, objects["space_debris_01"].scale, WHITE);
        DrawModel(objects["space_debris_02"].model, objects["space_debris_02"].position, objects["space_debris_02"].scale, WHITE);
        DrawModel(objects["space_debris_03"].model, objects["space_debris_03"].position, objects["space_debris_03"].scale, WHITE);
        DrawModel(objects["space_debris_04"].model, objects["space_debris_04"].position, objects["space_debris_04"].scale, WHITE);
        DrawModel(objects["space_debris_05"].model, objects["space_debris_05"].position, objects["space_debris_05"].scale, WHITE);

        DrawBoundingBox(box, GREEN);
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
                source.y = it->height * (it->currentFrame / it->rows);
                source.width = (float)it->width;
                source.height = (float)it->height;
                Vector3 position = {it->position.x, it->position.y, 3};
                Vector3 up = {0, 1, 0};
                Vector2 scale = it->texture->scale;
                DrawBillboardPro(camera, it->texture->texture, source, position, up, scale, Vector2Zero(), it->rotation, WHITE);
                ++it;
            }
        }
        EndMode3D();

        BeginShaderMode(shaders["outline"]);
        DrawTexturePro(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(target.texture.height)}, Vector2Zero(), 0,
                       WHITE);
        EndShaderMode();
        DrawFPS(10, 10);
        DrawText("Use keys [LEFT][RIGHT][UP][DOWN][SPACE] to move and shoot", 10, 40, 20, DARKGRAY);
        // stats about selected object
        DrawText(TextFormat("Selected object: %s", selectedMyObjectName.c_str()), 10, 70, 20, GREEN);
        if (selectedMyObject != nullptr) {
            DrawText(
                TextFormat("Position: %.2f, %.2f, %.2f", selectedMyObject->position.x, selectedMyObject->position.y, selectedMyObject->position.z),
                10, 100, 20, GREEN);
            DrawText(TextFormat("Scale: %.2f", selectedMyObject->scale), 10, 130, 20, GREEN);
            DrawText(
                TextFormat("Rotation: %.2f, %.2f, %.2f", selectedMyObject->rotation.x, selectedMyObject->rotation.y, selectedMyObject->rotation.z),
                10, 160, 20, GREEN);
        }
        EndDrawing();
    }
    // Enable raylib log
    SetTraceLogLevel(LOG_INFO);

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

    // unloads all the music
    for (auto &music : musics) {
        UnloadMusicStream(music.second.music);
    }
    musics.clear();

    // unloads all the sounds
    for (auto &sound : sounds) {
        UnloadSound(sound.second);
    }
    sounds.clear();

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

    CloseAudioDevice();
    CloseWindow();
    // LoadingThread.join();
    
    return 0;
}

/* int main() {
    ecs::Archetype<ecs::Position, ecs::Velocity> archetype;
    ecs::entity_type entity = archetype.create_entity(ecs::Position{0, 0}, ecs::Velocity{1, 2});

    auto [position, velocity] = archetype.query<0, 1>(entity);
    //archetype.delete_entity(entity);
    if (archetype.getDeletedEntities().count(entity) == 0) {
        std::cout << "Entity: " << entity << " position: " << position.x << ", " << position.y << std::endl;
        std::cout << "Entity: " << entity << " position: " << velocity.x << ", " << velocity.y << std::endl;
    } else {
        std::cout << "The entity "<< entity << " is deleted" << std::endl;
    }

    return 0;
} */

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