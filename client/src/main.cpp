/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "rtype-client.hpp"

int main(int ac, char *av[]) {
    if (ac != 3)
        std::cout << "Usage: ./client [ip] [port]" << std::endl;

    // Initialization
    std::string ip = av[1];
    int port = std::stoi(av[2]);

    if (port < 0 || port > 65535) {
        std::cout << "Invalid port" << std::endl;
        return 84;
    }
    if (ip == "localhost")
        ip = "127.0.0.1";

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
    DrawText("Connecting to server...", screenWidth / 2, screenHeight / 2, 24, WHITE);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // load engine
    Engine engine;

    // check connection to server
    if (engine.getUdpClient()->disconnected() && !engine.getUdpClient()->connect(ip, port))
        return 84;

    // load assets
    engine.loadAssets("assets/assets.json");

    // disable esc closing the window
    SetExitKey(0);

    // Play main music
    engine.playMusic(1);

    // Pause menu buttons / slider
    bool pause = false;
    bool quit = false;
    engine.addButton("resume_button", "Resume", Rectangle{float(screenWidth / 2 - 100), 200, 200, 50}, pause);
    engine.addButton("quit_button", "Quit", Rectangle{float(screenWidth / 2 - 100), 300, 200, 50}, pause);
    engine.getButton("quit_button")->SetCallback([&engine, &quit]() {
        engine.playSound("decision");
        SetWindowTitle("Bye bye !");
        quit = true;
    });
    engine.getButton("resume_button")->SetCallback([&pause, &engine]() {
        pause = !pause;
        engine.setPause(pause);
    });
    engine.addSlider("music_volume", Rectangle{float(screenWidth - 200), 100, 100, 20}, engine.getMusicVolume(), 0, 2, pause);
    engine.addSlider("sound_volume", Rectangle{float(screenWidth - 200), 150, 100, 20}, engine.getSoundVolume(), 0, 2, pause);

    Rectangle bounds = {float(screenWidth - 200), 100, 100, 20};
    engine.addSlider("posX", bounds, nullptr, -10, 10, false);
    bounds.y += 30;
    engine.addSlider("posY", bounds, nullptr, -10, 10, false);
    bounds.y += 30;
    engine.addSlider("posZ", bounds, nullptr, -10, 10, false);
    bounds.y += 30;
    engine.addSlider("rotX", bounds, nullptr, -3, 3, false);
    bounds.y += 30;
    engine.addSlider("rotY", bounds, nullptr, -3, 3, false);
    bounds.y += 30;
    engine.addSlider("rotZ", bounds, nullptr, -3, 3, false);
    bounds.y += 30;
    engine.addSlider("scale", bounds, nullptr, .1, 3, false);
    bounds.y = 100;
    engine.getSlider("music_volume")->SetCallback([&engine](const float *value) { engine.setMusicVolume(*value); });
    engine.getSlider("sound_volume")->SetCallback([&engine](const float *value) { engine.setSoundVolume(*value); });
    engine.addLight(Vector3Zero());

    engine.getObject("corridor")->SetVelocity(Vector3{-speed, 0, 0});
    engine.getObject("E002")->PlayAnimation(0, true);

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
    bool sliderSelected;

    while (!quit && !WindowShouldClose()) {
        Vector3 corridorPos = engine.getObject("corridor")->GetPosition();
        if (corridorPos.x < -tp) {
            corridorPos.x = 0;
            engine.getObject("corridor")->SetPosition(corridorPos);
        }

        // update sliders _request
        if (selectedMyObject != nullptr) {
            selectedMyObjectPos = selectedMyObject->GetPosition();
            selectedMyObjectRot = selectedMyObject->GetRotationGoal();
            selectedMyObjectScale = selectedMyObject->GetScale();
        }

        // Update sliders
        sliderSelected = engine.updateSliders();

        // Update pause menu
        if (IsKeyPressed(KEY_ESCAPE)) {
            pause = !pause;
            engine.setPause(pause);
        }
        // Update buttons
        engine.updateButton("resume_button");
        engine.updateButton("quit_button");

        // Left click to raycast on the scene
        if (!sliderSelected && !pause && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
                    // update sliders
                    engine.getSlider("posX")->SetValue(&selectedMyObjectPos.x);
                    engine.getSlider("posY")->SetValue(&selectedMyObjectPos.y);
                    engine.getSlider("posZ")->SetValue(&selectedMyObjectPos.z);
                    engine.getSlider("rotX")->SetValue(&selectedMyObjectRot.x);
                    engine.getSlider("rotY")->SetValue(&selectedMyObjectRot.y);
                    engine.getSlider("rotZ")->SetValue(&selectedMyObjectRot.z);
                    engine.getSlider("scale")->SetValue(&selectedMyObjectScale);
                    break;
                }
            }
            if (!collision.hit) {
                selectedMyObjectName = "";
                selectedMyObject = nullptr;
                engine.getSlider("posX")->SetEnabled(false);
                engine.getSlider("posY")->SetEnabled(false);
                engine.getSlider("posZ")->SetEnabled(false);
                engine.getSlider("rotX")->SetEnabled(false);
                engine.getSlider("rotY")->SetEnabled(false);
                engine.getSlider("rotZ")->SetEnabled(false);
                engine.getSlider("scale")->SetEnabled(false);
            }
        }

        // update selected object
        if (selectedMyObject != nullptr) {
            selectedMyObject->SetPosition(selectedMyObjectPos);
            selectedMyObject->SetRotationGoal(selectedMyObjectRot);
            selectedMyObject->SetScale(selectedMyObjectScale);
        }

        if (!pause)
            moveSpaceship(engine.getObject("R9A"), engine.getUdpClient());

        if (!pause && IsKeyPressed(KEY_SPACE)) {
            Vector3 bulletVelocity = Vector3Zero();
            bulletVelocity.x = bulletSpeed;
            bulletVelocity.y = -engine.getObject("R9A")->GetRotation().x * bulletSpeed * .5f;
            Vector3 bulletPos = engine.getObject("R9A")->GetPosition();
            engine.addBullet(bulletPos, bulletVelocity);
        }

        // Update camera
        engine.updateCamera();

        // Update music
        engine.updateMusic();

        if (!pause) {
            // Update bullets / particles
            engine.updateParticles2D();
            engine.updateParticles();
            engine.updateBullets();
            // update GameObjects
            engine.updateObjects();
        }

        engine.getLight(0)->setPosition(Vector3Add(engine.getObject("R9A")->GetPosition(), {0, 2, 10}));
        engine.updateLights();

        // update the light shader with the camera view position
        engine.updateShaderLocView("lighting");
        engine.updateShaderLocView("normal");

        // update requests
        engine.updateUdpClient();

        // show requests
        // engine.getUdpClient()->show_requests();

        // Draw normal for outline shader
        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(WHITE);
        BeginMode3D(*engine.getCamera());
        // engine.setShaderObject("R9A", "normal");
        // auto players = engine.getUdpClient()->getPlayers();
        // for (int i = 0; i < players.size() && i < 3; i++) {
        //     auto player = players[i];
        //     if (player == nullptr)
        //         continue;
        //     auto ship = "R9A" + std::to_string(i + 2);
        //     engine.setShaderObject(ship, "normal");
        //     engine.drawObject(ship);
        // }
        // engine.drawObject("R9A");
        EndMode3D();
        EndTextureMode();

        // Draw the scene with lights
        ClearBackground(BLACK);
        BeginMode3D(*engine.getCamera());
        // set shaders
        // engine.setShaderObject("R9A", "lighting");
        // for (int i = 0;  i < players.size() && i < 3; i++) {
        //    auto player = players[i];
        //    if (player == nullptr)
        //        continue;
        //    auto ship = "R9A" + std::to_string(i + 2);
        //    engine.setShaderObject(ship, "normal");
        //}

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
        // draw buttons
        engine.drawButtons();
        EndDrawing();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    UnloadRenderTexture(target);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
