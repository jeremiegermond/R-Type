/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include "rtype-client.hpp"
#include "core/archetype.hpp"
#include "core/components.hpp"

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
    std::unordered_map<std::string, GameObject *> objects;
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

    // Set shaders
    objects["corridor"]->SetShader(shaders["lighting"]);
    objects["rock_01"]->SetShader(shaders["lighting"]);
    objects["rock_02"]->SetShader(shaders["lighting"]);
    objects["space_debris_01"]->SetShader(shaders["lighting"]);
    objects["space_debris_02"]->SetShader(shaders["lighting"]);
    objects["space_debris_03"]->SetShader(shaders["lighting"]);
    objects["space_debris_04"]->SetShader(shaders["lighting"]);
    objects["space_debris_05"]->SetShader(shaders["lighting"]);

    objects["corridor"]->SetVelocity(Vector3{-speed, 0, 0});

    objects["e1116"]->PlayAnimation(0, false);
    // Disable raylib debug messages
    SetTraceLogLevel(LOG_NONE);

    // temp
    BoundingBox box;
    Ray ray;
    Vector2 mouse;
    RayCollision collision;

    // selected myObject
    std::string selectedMyObjectName;
    GameObject *selectedMyObject = nullptr;
    Vector3 selectedMyObjectPos;
    Vector3 selectedMyObjectRot;
    float selectedMyObjectScale;
    Rectangle bounds = {float(screenWidth - 200), 100, 100, 20};
    std::vector<Slider> sliders;
    bool sliderSelected = false;

    while (!WindowShouldClose()) {
        box = objects["spaceship1"]->GetMyObjectBoundingBox();
        Vector3 corridorPos = objects["corridor"]->GetPosition();
        if (corridorPos.x < -tp) {
            corridorPos.x = 0;
            objects["corridor"]->SetPosition(corridorPos);
        }

        // update sliders data
        if (selectedMyObject != nullptr) {
            selectedMyObjectPos = selectedMyObject->GetPosition();
            selectedMyObjectRot = selectedMyObject->GetRotationGoal();
            selectedMyObjectScale = selectedMyObject->GetScale();
        }
        // Update sliders
        for (auto &slider : sliders) {
            sliderSelected = slider.UpdateSlider();
            if (sliderSelected)
                break;
        }

        // Left click to raycast on the scene
        if (!sliderSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mouse = GetMousePosition();
            ray = GetMouseRay(mouse, camera);
            // cycle through all objects of the scene
            for (auto &object : objects) {
                // check if the ray hits the object
                BoundingBox objectBox = object.second->GetMyObjectBoundingBox();
                collision = GetRayCollisionBox(ray, objectBox);
                if (collision.hit) {
                    std::cout << "hit " << object.first << std::endl;
                    selectedMyObjectName = object.first;
                    selectedMyObject = object.second;
                    selectedMyObjectPos = selectedMyObject->GetPosition();
                    selectedMyObjectRot = selectedMyObject->GetRotationGoal();
                    selectedMyObjectScale = selectedMyObject->GetScale();
                    // clear sliders
                    sliders.clear();
                    // create sliders
                    sliders.emplace_back("posX", bounds, &selectedMyObjectPos.x, -10, 10);
                    bounds.y += 30;
                    sliders.emplace_back("posY", bounds, &selectedMyObjectPos.y, -10, 10);
                    bounds.y += 30;
                    sliders.emplace_back("posZ", bounds, &selectedMyObjectPos.z, -10, 10);
                    bounds.y += 30;
                    sliders.emplace_back("rotX", bounds, &selectedMyObjectRot.x, -3, 3);
                    bounds.y += 30;
                    sliders.emplace_back("rotY", bounds, &selectedMyObjectRot.y, -3, 3);
                    bounds.y += 30;
                    sliders.emplace_back("rotZ", bounds, &selectedMyObjectRot.z, -3, 3);
                    bounds.y += 30;
                    sliders.emplace_back("scale", bounds, &selectedMyObjectScale, .1, 3);
                    bounds.y = 100;
                    break;
                }
            }
            if (!collision.hit) {
                selectedMyObjectName = "";
                selectedMyObject = nullptr;
                sliders.clear();
            }
        }

        // update selected object
        if (selectedMyObject != nullptr) {
            selectedMyObject->SetPosition(selectedMyObjectPos);
            selectedMyObject->SetRotationGoal(selectedMyObjectRot);
            selectedMyObject->SetScale(selectedMyObjectScale);
        }

        moveSpaceship(objects["spaceship1"]);
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
            Bullet bullet = {objects["spaceship1"]->GetPosition(),
                             {objects["spaceship1"]->GetRotation().y < 0 ? -bulletSpeed : bulletSpeed,
                              -objects["spaceship1"]->GetRotation().x * bulletSpeed * .2f, 0},
                             bulletSize,
                             newLightIndex,
                             lights.back()};
            availableLights.erase(newLightIndex);
            bullets.emplace_back(bullet);
            lights.back().get()->setPosition(objects["spaceship1"]->GetPosition());
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

        BoundingBox enemyBounds = objects["e1116"]->GetMyObjectBoundingBox(Vector3{0, .5, 0});
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->position = Vector3Add(it->position, it->velocity);
            BoundingBox bulletBounds = GetBoundingBoxAroundPoint(it->position, .11);
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

        lights[0]->setPosition(Vector3Add(objects["spaceship1"]->GetPosition(), {0, 2, 10}));
        for (auto &light : lights) {
            light->UpdateLightValues(shaders["lighting"]);
        }

        // update the light shader with the camera view position
        SetShaderValue(shaders["lighting"], shaders["lighting"].locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shaders["normal"], shaders["normal"].locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // update GameObjects
        for (auto &object : objects) {
            object.second->Update();
        }

        // Draw normal for outline shader
        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(camera);
        objects["spaceship1"]->SetShader(shaders["normal"]);
        objects["e1116"]->SetShader(shaders["normal"]);
        objects["spaceship1"]->Draw(camera);
        objects["e1116"]->Draw(camera);
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(BLACK);
        BeginMode3D(camera);
        // set shaders
        objects["spaceship1"]->SetShader(shaders["lighting"]);
        objects["e1116"]->SetShader(shaders["lighting"]);

        // draw GameObjects
        for (auto &object : objects) {
            object.second->Draw(camera);
        }
        DrawBoundingBox(box, GREEN);

        // Draw rotating corridor
        objects["corridor"]->Draw(camera, Vector3{-tp * 2, 0, 0});
        objects["corridor"]->Draw(camera, Vector3{-tp, 0, 0});
        objects["corridor"]->Draw(camera, Vector3{tp, 0, 0});
        objects["corridor"]->Draw(camera, Vector3{tp * 2, 0, 0});
        // DrawBoundingBox(enemyBounds, RED);
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
            DrawText(TextFormat("Position: %.2f, %.2f, %.2f", selectedMyObjectPos.x, selectedMyObjectPos.y, selectedMyObjectPos.z), 10, 100, 20,
                     GREEN);
            DrawText(TextFormat("Scale: %.2f", selectedMyObjectScale), 10, 130, 20, GREEN);
            DrawText(TextFormat("Rotation Goal: %.2f, %.2f, %.2f", selectedMyObjectRot.x, selectedMyObjectRot.y, selectedMyObjectRot.z), 10, 160, 20,
                     GREEN);
        }
        // draw sliders
        for (auto &slider : sliders) {
            slider.Draw();
        }
        EndDrawing();
    }
    // Enable raylib log
    SetTraceLogLevel(LOG_INFO);

    UnloadModel(cube);
    UnloadRenderTexture(target);

    // unloads sliders
    sliders.clear();

    // unloads all the models
    for (auto &object : objects) {
        delete object.second;
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
}

int main() {
    ecs::SparseMap<int> sm;

    sm.insert(5, 10);
    sm.insert(8, 20);
    sm.insert(1, 30);
    sm.insert(3, 40);

    std::cout << sm[5] << std::endl; // Outputs 10
    std::cout << sm[8] << std::endl; // Outputs 20
    std::cout << sm[1] << std::endl; // Outputs 30
    std::cout << sm[3] << std::endl; // Outputs 40

    sm.erase(5);
    std::cout << sm[5] << std::endl; // Outputs 0

    sm.insert(5, 20);
    std::cout << sm[5] << std::endl; // Outputs 20

    return 0;
} */