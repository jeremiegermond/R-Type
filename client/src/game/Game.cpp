/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr), _udpClient(nullptr), _pObjectArchetype(nullptr) {}

void Game::initGame() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    _ecsManager = std::make_shared<ECSManager>();
    _ecsManager->init();
    _pObjectArchetype = _ecsManager->getArchetype<ObjectArchetype>("Object");
    loadAssets("assets/assets.json");
    loadEntities("assets/levels/level_01.json");
    _pObjectArchetype->getComponent<Engine::CObject>(_gameEntities["corridor"]).setActive(true);
}

void Game::updateGame() {
    _ecsManager->update();

    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
    }
    Camera3D camera = {0};
    camera.position = {0, 0, 30};
    camera.target = Vector3Zero();
    camera.up = {0, 1, 0};
    camera.fovy = 20;
    camera.projection = CAMERA_PERSPECTIVE;
    BeginMode3D(camera);
    for (auto &entity : _gameEntities) {
        auto object = _pObjectArchetype->getComponent<Engine::CObject>(entity.second);
        if (!object.isActive())
            continue;
        auto [model, position, scale, velocity] =
            _pObjectArchetype->getComponent<pModel, Engine::CPosition, Engine::CScale, Engine::CVelocity>(entity.second);
        velocity.doUpdate();
        position.addPosition(velocity.getSpeed());
        if (model != nullptr) {
            auto v3 = position.getPosition();
            DrawModel(model->getModel(), v3, scale.getScale(), WHITE);
            if (entity.first == "corridor") {
                if (v3.x < -7.22)
                    position.setPosition({0, v3.y, v3.z});
                v3.x -= 7.22 * 2;
                DrawModel(model->getModel(), v3, scale.getScale(), WHITE);
                v3.x += 7.22;
                DrawModel(model->getModel(), v3, scale.getScale(), WHITE);
                v3.x += 7.22 * 2;
                DrawModel(model->getModel(), v3, scale.getScale(), WHITE);
                v3.x += 7.22;
                DrawModel(model->getModel(), v3, scale.getScale(), WHITE);
            }
        }
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
    if (data.contains("objects")) {
        for (auto &object : data["objects"]) {
            if (!object.contains("name")) {
                TraceLog(LOG_WARNING, "Failed to load object: missing name");
                TraceLog(LOG_WARNING, "Skipping object: %s", object.dump().c_str());
                continue;
            }
            std::string name = object["name"];
            auto entity = _pObjectArchetype->createEntity(Engine::CScale(1), Engine::CVelocity(Vector3Zero()), Engine::CObject(),
                                                          Engine::CPosition(Vector3Zero()), Engine::CRotation(Vector3Zero()));
            _gameEntities[name] = entity;
            auto [cObject, cPosition, cScale, cVelocity, cRotation] =
                _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CScale, Engine::CVelocity, Engine::CRotation>(entity);

            if (object.contains("model")) {
                auto model = object["model"];
                if (!_models.contains(model))
                    continue;
                _pObjectArchetype->addComponent(entity, pModel(_models[model]));
            }
            if (object.contains("tags")) {
                auto tags = object["tags"];
                for (auto &tag : tags) {
                    cObject.setTag(tag);
                }
            }
            if (object.contains("position") && object["position"].size() == 3) {
                auto positionStr = object["position"];
                auto position = Vector3{positionStr[0], positionStr[1], positionStr[2]};
                cPosition.setPosition(position);
            }
            if (object.contains("rotation") && object["rotation"].size() == 3) {
                auto rotationStr = object["rotation"];
                auto rotation = Vector3{rotationStr[0], rotationStr[1], rotationStr[2]};
                cRotation.setRotation(rotation);
            }
            if (object.contains("scale")) {
                auto scale = object["scale"];
                cScale.setScale(scale);
            }
            if (object.contains("animation")) {
                auto animation = object["animation"];
                if (!_animations.contains(animation))
                    continue;
                _pObjectArchetype->addComponent(entity, pAnimation(_animations[animation]));
            }
            if (object.contains("velocity") && object["velocity"].size() == 3) {
                auto velocityStr = object["velocity"];
                auto velocity = Vector3{velocityStr[0], velocityStr[1], velocityStr[2]};
                cVelocity.setSpeed(velocity);
            }
        }
    }
}
