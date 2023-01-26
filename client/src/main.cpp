/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#include "core/archetype.hpp"
#include "core/components.hpp"
#include "rtype-client.hpp"

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

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    float speed = .01;
    float tp = 7.22;

    float bulletSpeed = .2;

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

    // load engine
    Engine engine("assets/assets.json");

    // Play menu music
    engine.playMusic("02-Main_Menu");

    // ambient light
    // float lightPos[4] = {.1, .1, .1, 1};
    // int ambientLoc = GetShaderLocation(shaders["lighting"], "ambient");
    // SetShaderValue(shaders["lighting"], ambientLoc, lightPos, SHADER_UNIFORM_VEC4);

    engine.addLight(Vector3Zero());

    engine.getObject("corridor")->SetVelocity(Vector3{-speed, 0, 0});

    engine.getObject("e1116")->PlayAnimation(0, false);

    // temp
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
    bool sliderSelected;

    while (!WindowShouldClose()) {
        Vector3 corridorPos = engine.getObject("corridor")->GetPosition();
        if (corridorPos.x < -tp) {
            corridorPos.x = 0;
            engine.getObject("corridor")->SetPosition(corridorPos);
        }

        // update sliders data
        if (selectedMyObject != nullptr) {
            selectedMyObjectPos = selectedMyObject->GetPosition();
            selectedMyObjectRot = selectedMyObject->GetRotationGoal();
            selectedMyObjectScale = selectedMyObject->GetScale();
        }
        // Update sliders
        sliderSelected = engine.updateSliders();

        // Left click to raycast on the scene
        if (!sliderSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            engine.clearSliders();
            mouse = GetMousePosition();
            ray = GetMouseRay(mouse, *engine.getCamera());
            // cycle through all objects of the scene
            for (auto &object : engine.getObjects()) {
                if (object.second->IsTagged("background")) {
                    continue;
                }
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
                    // create sliders
                    engine.addSlider("posX", bounds, &selectedMyObjectPos.x, -10, 10);
                    bounds.y += 30;
                    engine.addSlider("posY", bounds, &selectedMyObjectPos.y, -10, 10);
                    bounds.y += 30;
                    engine.addSlider("posZ", bounds, &selectedMyObjectPos.z, -10, 10);
                    bounds.y += 30;
                    engine.addSlider("rotX", bounds, &selectedMyObjectRot.x, -3, 3);
                    bounds.y += 30;
                    engine.addSlider("rotY", bounds, &selectedMyObjectRot.y, -3, 3);
                    bounds.y += 30;
                    engine.addSlider("rotZ", bounds, &selectedMyObjectRot.z, -3, 3);
                    bounds.y += 30;
                    engine.addSlider("scale", bounds, &selectedMyObjectScale, .1, 3);
                    bounds.y = 100;
                    break;
                }
            }
            if (!collision.hit) {
                selectedMyObjectName = "";
                selectedMyObject = nullptr;
            }
        }

        // update selected object
        if (selectedMyObject != nullptr) {
            selectedMyObject->SetPosition(selectedMyObjectPos);
            selectedMyObject->SetRotationGoal(selectedMyObjectRot);
            selectedMyObject->SetScale(selectedMyObjectScale);
        }

        moveSpaceship(engine.getObject("spaceship1"));
        if (IsKeyPressed(KEY_P)) {
            std::cout << "Some debug infos:" << std::endl;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            Vector3 bulletVelocity = Vector3Zero();
            bulletVelocity.x = bulletSpeed;
            bulletVelocity.y = -engine.getObject("spaceship1")->GetRotation().x * bulletSpeed * .5f;
            Vector3 bulletPos = engine.getObject("spaceship1")->GetPosition();
            engine.addBullet(bulletPos, bulletVelocity);
        }

        // Update camera
        engine.updateCamera();

        // Update music
        engine.updateMusic();

        // Update bullets / particles
        engine.updateParticles2D();
        engine.updateParticles();
        engine.updateBullets();

        engine.getLight(0)->setPosition(Vector3Add(engine.getObject("spaceship1")->GetPosition(), {0, 2, 10}));
        engine.updateLights();

        // update the light shader with the camera view position
        engine.updateShaderLocView("lighting");
        engine.updateShaderLocView("normal");

        // update GameObjects
        engine.updateObjects();

        // Draw normal for outline shader
        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(*engine.getCamera());
        engine.setShaderObject("spaceship1", "normal");
        engine.setShaderObject("e1116", "normal");
        engine.drawObject("spaceship1");
        engine.drawObject("e1116");
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(BLACK);
        BeginMode3D(*engine.getCamera());
        // set shaders
        engine.setShaderObject("spaceship1", "lighting");
        engine.setShaderObject("e1116", "lighting");

        // draw GameObjects
        engine.drawObjects();

        // Draw selected object bounding box
        if (selectedMyObject != nullptr) {
            DrawBoundingBox(selectedMyObject->GetMyObjectBoundingBox(), GREEN);
        }

        // Draw rotating corridor
        engine.drawObject("corridor", Vector3{-tp * 2, 0, 0});
        engine.drawObject("corridor", Vector3{-tp, 0, 0});
        engine.drawObject("corridor", Vector3{tp, 0, 0});
        engine.drawObject("corridor", Vector3{tp * 2, 0, 0});

        // Draw light sphere
        engine.drawLightsSpheres();

        // Draw particles
        engine.drawParticles();

        // Draw bullets
        engine.drawBullets();

        // Draw particles2D
        engine.drawParticles2D();
        EndMode3D();

        engine.setShaderMode("outline");
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
        engine.drawSliders();
        EndDrawing();
    }
    UnloadRenderTexture(target);

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
