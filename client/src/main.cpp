/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "rtype-client.hpp"

/**
 * It's a game loop
 * 
 * @param ac The number of arguments passed to the program.
 * @param av the command line arguments
 * 
 * @return The return value is the number of bytes sent.
 */
int main(int ac, char *av[]) {
    std::string ip = "127.0.0.1";
    int port = 12345;
    if (ac == 3) {
        ip = av[1];
        port = std::stoi(av[2]);
    } else
        std::cout << "Usage: ./client [ip] [port]" << std::endl;

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

    float speed = .5;
    float tp = 7.22;

    float bulletSpeed = 5;

    // load engine
    Engine engine;

    // Connect udp client
    engine.setUdpClient(new UdpClient(ip, port));

    Vector2 center = {float(screenWidth / 2), float(screenHeight / 2)};

    // check connection to server
    while (!engine.getUdpClient()->isConnected()) {
        BeginDrawing();
        ClearBackground(BLACK);
        Engine::drawText("Connecting to server...", 24, center, WHITE, true);
        EndDrawing();
        if (WindowShouldClose())
            return 0;
    }

    // check if user closed window

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

    engine.getObject("corridor")->setVelocity(Vector3{-speed, 0, 0});
    engine.getObject("E002")->playAnimation(0, true);

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
        Vector3 corridorPos = engine.getObject("corridor")->getPosition();
        if (corridorPos.x < -tp) {
            corridorPos.x = 0;
            engine.getObject("corridor")->setPosition(corridorPos);
        }

        // update sliders _request
        if (selectedMyObject != nullptr) {
            selectedMyObjectPos = selectedMyObject->getPosition();
            selectedMyObjectRot = selectedMyObject->getRotationGoal();
            selectedMyObjectScale = selectedMyObject->getScale();
        }

        // update sliders
        sliderSelected = engine.updateSliders();

        // update pause menu
        if (IsKeyPressed(KEY_ESCAPE)) {
            pause = !pause;
            engine.setPause(pause);
        }
        // update buttons
        engine.updateButton("resume_button");
        engine.updateButton("quit_button");

        // Left click to raycast on the scene
        if (!sliderSelected && !pause && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mouse = GetMousePosition();
            ray = GetMouseRay(mouse, *engine.getCamera());
            // cycle through all objects of the scene
            for (auto &object : engine.getObjects()) {
                if (object.second->isTagged("background")) {
                    continue;
                }
                // check if the ray hits the object
                BoundingBox objectBox = object.second->getBoundingBox();
                collision = GetRayCollisionBox(ray, objectBox);
                if (collision.hit) {
                    std::cout << "hit " << object.first << std::endl;
                    selectedMyObjectName = object.first;
                    selectedMyObject = object.second;
                    selectedMyObjectPos = selectedMyObject->getPosition();
                    selectedMyObjectRot = selectedMyObject->getRotationGoal();
                    selectedMyObjectScale = selectedMyObject->getScale();
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
            selectedMyObject->setPosition(selectedMyObjectPos);
            selectedMyObject->setRotationGoal(selectedMyObjectRot);
            selectedMyObject->setScale(selectedMyObjectScale);
        }

        if (!pause)
            moveSpaceship(engine.getPlayerShip(), engine.getUdpClient());

        if (!pause && IsKeyPressed(KEY_SPACE)) {
            Vector3 bulletVelocity = Vector3Zero();
            bulletVelocity.x = bulletSpeed;
            // bulletVelocity.y = -engine.getPlayerShip()->getRotation().x * bulletSpeed * .5f;
            Vector3 bulletPosition = engine.getPlayerShip()->getPosition();
            bulletPosition.x += .5;
            engine.getUdpClient()->send("shoot");
            engine.addBullet(bulletPosition, bulletVelocity);
        }

        // update camera
        engine.updateCamera();

        // update music
        engine.updateMusic();

        // update bullets / particles
        engine.updateParticles2D();
        engine.updateParticles();
        engine.updateBullets();
        // update GameObjects
        engine.updateObjects();

        engine.getLight(0)->setPosition(Vector3Add(engine.getPlayerShip()->getPosition(), {0, 2, 10}));
        engine.updateLights();

        // update the light shader with the camera view position
        engine.updateShaderLocView("lighting");
        engine.updateShaderLocView("normal");

        // update requests
        engine.updateUdpClient();

        // update enemies
        engine.updateEnemies();

        // draw normal for outline shader
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

        // draw the scene with lights
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

        // draw enemies
        engine.drawEnemies();

        // draw selected object bounding box
        if (selectedMyObject != nullptr) {
            DrawBoundingBox(selectedMyObject->getBoundingBox(), GREEN);
        }

        // draw rotating corridor
        engine.drawObject("corridor", Vector3{-tp * 2, 0, 0});
        engine.drawObject("corridor", Vector3{-tp, 0, 0});
        engine.drawObject("corridor", Vector3{tp, 0, 0});
        engine.drawObject("corridor", Vector3{tp * 2, 0, 0});

        // draw light sphere
        engine.drawLightsSpheres();

        // draw particles
        engine.drawParticles();

        // draw bullets
        engine.drawBullets();

        // draw particles2D
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

        // draw connection status flashing
        if (!engine.getUdpClient()->isConnected() && int(GetTime() * 2) % 2) {
            Engine::drawText("Connection lost", 40, center, RED, true);
        }
        EndDrawing();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    UnloadRenderTexture(target);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
