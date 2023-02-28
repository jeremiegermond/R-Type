/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr), _udpClient(nullptr), _pObjectArchetype(nullptr), _camera({0}) {}

void Game::initGame() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    rlDisableBackfaceCulling();
    _camera.position = {0, 0, 30};
    _camera.target = Vector3Zero();
    _camera.up = {0, 1, 0};
    _camera.fovy = 20;
    _camera.projection = CAMERA_PERSPECTIVE;
    _ecsManager = std::make_shared<ECSManager>();
    _ecsManager->init();
    _pObjectArchetype = _ecsManager->getArchetype<ObjectArchetype>("Object");
    loadAssets("assets/assets.json");
    loadEntities("assets/levels/level_01.json");
    _pObjectArchetype->getComponent<Engine::CObject>(_gameEntities["R9A1"]).setActive(true);
    _pObjectArchetype->getComponent<Engine::CObject>(_gameEntities["corridor"]).setActive(true);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        _lightIds.insert(i);
    }
    addLight();
    auto shader = *_shaders["lighting"]->getShader();
    auto ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = {.1, .1, .1, 1};
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
}

void Game::updateNetwork() {
    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
        if (Engine::Regex::isMatch(msg, "id:[0-9]+")) {
            _playerId = std::stoi(Engine::Regex::getMatch(msg, "id:([0-9]+)", 1));
            auto name = "R9A" + std::to_string(_playerId);
            auto [shipPosition, ship] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(true);
            shipPosition.setPosition(Vector3Zero());
        } else if (Engine::Regex::isMatch(msg, "new:[0-9]+," NB_R "," NB_R)) {
            auto match = Engine::Regex::getMatches(msg, "new:([0-9]+)," NB_R "," NB_R);
            auto newShipId = std::stoi(match[1]);
            auto newShipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto name = "R9A" + std::to_string(newShipId);
            auto [shipPosition, ship] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(true);
            shipPosition.setPosition(newShipPosition);
        } else if (Engine::Regex::isMatch(msg, "move:[0-9]+," NB_R "," NB_R)) {
            auto match = Engine::Regex::getMatches(msg, "move:([0-9]+)," NB_R "," NB_R);
            auto shipId = match[1];
            auto shipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto name = "R9A" + shipId;
            auto [shipPos, ship] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            shipPos.setPosition(shipPosition);
        } else if (Engine::Regex::isMatch(msg, "del:[0-9]+")) {
            auto deletedShipId = Engine::Regex::getMatch(msg, "del:([0-9]+)", 1);
            auto name = "R9A" + deletedShipId;
            auto [shipPosition, ship] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(false);
            shipPosition.setPosition({-100, -100, 0});
        } else if (Engine::Regex::isMatch(msg, "shoot:[0-9]+")) {
            auto id = Engine::Regex::getMatch(msg, "shoot:([0-9]+)", 1);
            auto name = "R9A" + id;
            auto [shipPosition, ship] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            auto bulletPosition = shipPosition.getPosition();
            bulletPosition.x += 1;
            auto bulletVelocity = Vector3Zero();
            bulletVelocity.x = 5;
            addBullet(bulletPosition, bulletVelocity);
        } else if (Engine::Regex::isMatch(msg, "spawn:[0-9]+," NB_R "," NB_R "," NB_R "," NB_R ",[0-9]+")) {
            auto match = Engine::Regex::getMatches(msg, "spawn:([0-9]+)," NB_R "," NB_R "," NB_R "," NB_R ",([0-9]+)");
            auto newEnemyId = std::stoi(match[1]);
            auto newEnemyPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto newEnemyVelocity = Vector3{std::stof(match[4]), std::stof(match[5]), 0};
            auto newEnemyLife = std::stoi(match[6]);
            // std::cout << "New enemy: " << newEnemyId << " at " << newEnemyPosition.x << " " << newEnemyPosition.y << std::endl;
            // addEnemy(newEnemyId, newEnemyPosition, newEnemyVelocity, newEnemyLife);
        } else if (Engine::Regex::isMatch(msg, "dead:[0-9]+")) {
            auto id = std::stoi(Engine::Regex::getMatch(msg, "dead:([0-9]+)", 1));
            // std::cout << "Dead enemy: " << id << std::endl;
            // if (_enemies.find(id) == _enemies.end())
            //    return;
            // auto enemy = _enemies[id];
            // auto enemyPosition = enemy->getPosition();
            // enemyPosition.z += 1.5;
            // addParticle2D("explosion", enemyPosition, float(sin(GetTime() * 10) * 90), 4);
            // erase pointer
            // delete enemy;
            //_enemies.erase(id);
        }
    }
}

void Game::updateGame() {
    _ecsManager->update();
    updateNetwork();
    movePlayer();
    updatePlayer();
    if (_shaders.contains("lighting")) {
        auto playerPosition = _pObjectArchetype->getComponent<Engine::CPosition>(_gameEntities["R9A" + std::to_string(_playerId)]).getPosition();
        playerPosition = Vector3Add(playerPosition, {0, 2, 10});
        _lights[0].setPosition(playerPosition);
    }
    updateLights();
    drawGame();
}

void Game::destroyGame() {
    _ecsManager->destroy();
    _ecsManager = nullptr;
}

void Game::initSocket(const std::string &ip, unsigned short port) { _udpClient = std::make_shared<UdpClient>(ip, port); }

void Game::drawGame() {
    BeginMode3D(_camera);
    if (_gameEntities.contains("corridor")) {
        auto corridor = _gameEntities["corridor"];
        auto [cPosition, cObject] = _pObjectArchetype->getComponent<Engine::CPosition, Engine::CObject>(corridor);
        auto position = cPosition.getPosition();
        if (position.x < -7.22) {
            cPosition.setPosition({0, position.y, position.z});
        }
        drawEntity(corridor, {-7.22 * 2, 0, 0});
        drawEntity(corridor, {-7.22, 0, 0});
        drawEntity(corridor, {7.22, 0, 0});
        drawEntity(corridor, {7.22 * 2, 0, 0});
    }
    for (auto &entity : _gameEntities) {
        auto object = _pObjectArchetype->getComponent<Engine::CObject>(entity.second);
        if (!object.isActive())
            continue;
        updateEntity(entity.second);
        drawEntity(entity.second);
    }
    for (auto &bullet : _bullets) {
        updateEntity(bullet);
        drawEntity(bullet);
    }
    EndMode3D();
}
