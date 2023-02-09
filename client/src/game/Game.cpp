/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr), _udpClient(nullptr) {}

void Game::initGame() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    _ecsManager = std::make_shared<ECSManager>();
    _ecsManager->init();
    loadAssets("assets/assets.json");
    loadEntities("assets/levels/level_01.json");
}

void Game::updateGame() {
    _ecsManager->update();

    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
    }
    Camera3D camera = {0};
    camera.position = {0.0f, 10.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    BeginMode3D(camera);
    auto objectFactory = _ecsManager->getArchetype<ObjectArchetype>("Object");
    for (auto &entity : _gameEntities) {
        auto object = objectFactory->getComponent<Engine::CObject>(entity.second);
        if (!object.hasTag("background"))
            continue;
        auto model = objectFactory->getComponent<pModel>(entity.second);
        auto position = objectFactory->getComponent<Engine::CPosition>(entity.second);
        if (model != nullptr)
            DrawModel(model->getModel(), position.getPosition(), 1, WHITE);
    }
    EndMode3D();
}

void Game::destroyGame() {
    _ecsManager->destroy();
    _ecsManager = nullptr;
}

void Game::initSocket(const std::string &ip, unsigned short port) { _udpClient = std::make_shared<UdpClient>(ip, port); }

void Game::loadEntities(const std::string &path) {
    if (path.empty())
        throw std::runtime_error("Invalid path to entities file");
    if (!FileExists(path.c_str()))
        throw std::runtime_error("Entities file not found");
    std::ifstream f(path);
    json data = json::parse(f);
    auto objectFactory = _ecsManager->getArchetype<ObjectArchetype>("Object");

    if (data.contains("objects")) {
        for (auto &object : data["objects"]) {
            if (!object.contains("name")) {
                TraceLog(LOG_WARNING, "Failed to load object: missing name");
                TraceLog(LOG_WARNING, "Skipping object: %s", object.dump().c_str());
                continue;
            }
            std::string name = object["name"];
            auto entity = objectFactory->createEntity();
            _gameEntities[name] = entity;
            auto cObject = Engine::CObject();

            if (object.contains("model")) {
                auto model = object["model"];
                if (!_models.contains(model))
                    continue;
                objectFactory->addComponent(entity, pModel(_models[model]));
            }
            if (object.contains("tags")) {
                auto tags = object["tags"];
                for (auto &tag : tags) {
                    cObject.setTag(tag);
                }
            }
            objectFactory->addComponent(entity, Engine::CObject(cObject));
        }
    }
}
