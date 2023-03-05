/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.cpp
*/

#include "game/Game.hpp"

Game::Game() : _ecsManager(nullptr), _camera(), _udpClient(nullptr), _gameState(GameState::MENU), _playerId(1), _gameOver(false), _selectedRoom(0) {}

void Game::initGame() {
    rlDisableBackfaceCulling();
    _camera = _pCameraArchetype.createEntity(Engine::CObject(), CCamera());
    auto [obj, camera] = _pCameraArchetype.getComponent<Engine::CObject, CCamera>(_camera);
    camera.setCamera(Camera3D{0});
    camera.setPosition({0, 0, 30});
    camera.setTarget(Vector3Zero());
    camera.setUp({0, 1, 0});
    camera.setFovy(20);
    camera.setProjection(CAMERA_PERSPECTIVE);
    loadAssets("assets/assets.json");
    loadEntities("assets/levels/menu.json");
    playMusic("02-Main_Menu");
    for (int i = 0; i < MAX_LIGHTS; i++) {
        _lightIds.insert(i);
    }
    addLight();
    auto shader = *_shaders["lighting"]->getShader();
    auto ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = {.1, .1, .1, 1};
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
}

void Game::updateGame() {
    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }
    if (_gameState == GameState::MENU) {
        getRooms();
        updateMenu();
    } else {
        updateGameplay();
        drawGame();
    }
    updateMusic();
    drawUI();
}

void Game::destroyGame() {}

void Game::initSocket(const std::string &ip, unsigned short port) {
    _udpClient = std::make_shared<UdpClient>(ip, port);
    _udpClient->start();
    _udpClient->send("getRooms");
}

void Game::drawGame() {
    BeginMode3D(_pCameraArchetype.getComponent<CCamera>(_camera).getCamera());
    if (_gameEntities.contains("corridor")) {
        auto corridor = _gameEntities["corridor"];
        auto [cPosition, cObject] = _objectArchetype.getComponent<Engine::CPosition, Engine::CObject>(corridor);
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
        auto object = _objectArchetype.getComponent<Engine::CObject>(entity.second);
        if (!object.isActive())
            continue;
        updateEntity(entity.second);
        drawEntity(entity.second);
    }
    for (auto &enemy : _enemies) {
        drawEntity(enemy.second);
    }
    for (auto &bullet : _bullets) {
        drawEntity(bullet);
    }
    for (auto &animatedSprite : _animatedSprites) {
        drawTexture(animatedSprite);
    }
    EndMode3D();
}

void Game::setGameState(GameState state) {
    if (state == GameState::MENU) {
        _udpClient->setPort(4242);
        playMusic("02-Main_Menu");
        _selectedRoom = 0;
        _udpClient->send("getRooms");
    }
    _gameState = state;
}

void Game::updateMenu() {
    if (_uiElements.contains("player_input") && _uiElements.contains("play_button")) {
        auto [input, inputText, inputColor] = _pUIArchetype.getComponent<Engine::CObject, CText, CColor>(_uiElements["player_input"]);
        auto [buttonObject, buttonColor] = _pUIArchetype.getComponent<Engine::CObject, CColor>(_uiElements["play_button"]);
        if (input.hasTag("selected")) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
                    inputText.setText("");
                } else {
                    auto text = inputText.getText();
                    if (!text.empty()) {
                        text.pop_back();
                        inputText.setText(text);
                    }
                }
            } else {
                int key = GetKeyPressed();
                if (key >= 32 && key <= 125 && inputText.getText().size() < 12) {
                    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                        key = toupper(key);
                    } else {
                        key = tolower(key);
                    }
                    std::string str;
                    str = char(key);
                    if ((!inputText.getText().empty() && Engine::Regex::isMatch(str, " ")) || Engine::Regex::isMatch(str, "[a-zA-Z0-9]+"))
                        inputText.setText(inputText.getText() + str);
                }
            }
        }
        if (inputText.getText().empty()) {
            inputColor.setColor({175, 175, 175, 255});
        } else {
            inputColor.setColor({50, 205, 50, 255});
        }
        if (inputText.getText().empty() || _selectedRoom == 0) {
            buttonColor.setColor({175, 175, 175, 255});
            buttonObject.setTag("disabled");
        } else {
            buttonColor.setColor({50, 205, 50, 255});
            buttonObject.removeTag("disabled");
        }
        if (!buttonObject.hasTag("disabled") && (buttonObject.hasTag("selected") || IsKeyPressed(KEY_ENTER))) {
            _playerName = inputText.getText();
            if (_udpClient) {
                _gameOver = false;
                _udpClient->setPort(_selectedRoom);
                _udpClient->send("name:" + _playerName);
                _rooms.clear();
            }
            _uiElements.clear();
            setGameState(GameState::GAME);
            loadEntities("assets/levels/level_01.json");
            _objectArchetype.getComponent<Engine::CObject>(_gameEntities["R9A1"]).setActive(true);
            _objectArchetype.getComponent<Engine::CObject>(_gameEntities["corridor"]).setActive(true);
            playMusic("01-Taking_off_again");
        }
    }
    if (_uiElements.contains("refresh_rooms")) {
        auto [button, bText] = _pUIArchetype.getComponent<Engine::CObject, CText>(_uiElements["refresh_rooms"]);
        if (!button.hasTag("disabled") && button.hasTag("selected")) {
            _rooms.clear();
            _udpClient->send("getRooms");
            button.removeTag("selected");
        }
    }
    if (_uiElements.contains("create_room")) {
        auto [button, bText] = _pUIArchetype.getComponent<Engine::CObject, CText>(_uiElements["create_room"]);
        if (!button.hasTag("disabled") && button.hasTag("selected")) {
            _rooms.clear();
            _udpClient->send("createRoom");
            _rooms.clear();
            _udpClient->send("getRooms");
            button.removeTag("selected");
        }
    }
    for (auto room : _rooms) {
        auto [button, bText, color] = _pUIArchetype.getComponent<Engine::CObject, CText, CColor>(room);
        if (button.hasTag("selected")) {
            button.removeTag("selected");
            auto port = Engine::Regex::getMatch(bText.getText(), "([0-9]+):", 1);
            _selectedRoom = std::stoi(port);
            color.setColor({50, 205, 50, 255});
            for (auto otherRoom : _rooms) {
                if (otherRoom != room) {
                    auto [otherButton, otherText, otherColor] = _pUIArchetype.getComponent<Engine::CObject, CText, CColor>(otherRoom);
                    otherColor.setColor({175, 175, 175, 255});
                }
            }
            break;
        }
    }
}

void Game::updateGameplay() {
    updateNetwork();
    movePlayer();
    updatePlayer();
    updateTextures();
    for (auto it = _enemies.begin(); it != _enemies.end();) {
        updateEntity(it->second);
        auto [health, position] = _objectArchetype.getComponent<CHealth, Engine::CPosition>(it->second);
        if (health.getHealth() <= 0 || position.getPosition().x < -10) {
            it = _enemies.erase(it);
        } else {
            ++it;
        }
    }
    updateBullets();
    if (_shaders.contains("lighting")) {
        auto playerPosition = _objectArchetype.getComponent<Engine::CPosition>(_gameEntities["R9A" + std::to_string(_playerId)]).getPosition();
        playerPosition = Vector3Add(playerPosition, {0, 2, 10});
        _lights[0].setPosition(playerPosition);
    }
    updateLights();
    if (_uiElements.contains("disconnect_button")) {
        auto cObject = _pUIArchetype.getComponent<Engine::CObject>(_uiElements["disconnect_button"]);
        if (cObject.hasTag("selected")) {
            _gameEntities.clear();
            _bullets.clear();
            _enemies.clear();
            _uiElements.clear();
            loadEntities("assets/levels/menu.json");
            if (_uiElements.contains("player_input")) {
                auto [pInput, cText] = _pUIArchetype.getComponent<Engine::CObject, CText>(_uiElements["player_input"]);
                cText.setText(_playerName);
            }
            setGameState(GameState::MENU);
        }
    }
}

void Game::drawUI() {
    DrawFPS(10, 10);
    bool isMouseOverUI = false;
    for (auto &ui : _uiElements) {
        isMouseOverUI += drawUIEntity(ui.second);
    }
    for (auto &ui : _rooms) {
        isMouseOverUI += drawUIEntity(ui);
    }
    if (!isMouseOverUI) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    } else {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}

bool Game::drawUIEntity(Engine::EntityId id) {
    bool isMouseOverUI = false;
    auto [cPosition, cObject, cText] = _pUIArchetype.getComponent<Engine::CPosition, Engine::CObject, CText>(id);
    auto screenSize = getWindowSize();
    auto position = Vector3Multiply(Vector3Scale(cPosition.getPosition(), .01), {screenSize.x, screenSize.y, 1});
    if (cObject.hasTag("input") || cObject.hasTag("button")) {
        auto [cBox, cColor] = _pUIArchetype.getComponent<CBox, CColor>(id);
        auto box = cBox.getBox();
        auto color = cColor.getColor();
        box.width *= .01f * screenSize.x;
        box.height *= .01f * screenSize.y;
        auto boxPosition = Vector3Subtract(position, {box.width * .5f, box.height * .5f, 0});
        auto boxFinal = Rectangle{boxPosition.x, boxPosition.y, box.width, box.height};
        auto hover = !cObject.hasTag("disabled") && CheckCollisionPointRec(GetMousePosition(), boxFinal);
        auto click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (hover) {
            isMouseOverUI = true;
            if (!cObject.hasTag("hover")) {
                cObject.setTag("hover");
                cBox.setScale(1.15);
                playSound("decision");
            }
            if (click && !cObject.hasTag("selected")) {
                cObject.setTag("selected");
                playSound("level_decision");
            }
        } else if (click) {
            if (cObject.hasTag("selected")) {
                cObject.removeTag("selected");
                playSound("cancel");
            }
            if (cObject.hasTag("hover")) {
                cObject.removeTag("hover");
                cBox.setScale(1);
            }
        } else {
            if (cObject.hasTag("hover")) {
                cObject.removeTag("hover");
                cBox.setScale(1);
            }
        }
        DrawRectangleRounded(boxFinal, 0.2f, 0, color);
    }
    if (cObject.hasTag("text")) {
        int fontSize = int(screenSize.y * .001f * float(cText.getFontSize()));
        auto text = cText.getText().c_str();
        position = Vector3Subtract(position, {float(MeasureText(text, fontSize)) * .5f, float(fontSize) * .5f, 0});
        DrawText(text, int(position.x), int(position.y), fontSize, WHITE);
        if (cObject.hasTag("input") && cObject.hasTag("selected")) {
            auto cursorPosition = Vector3Add(position, {float(MeasureText(text, fontSize)), 0, 0});
            cursorPosition.x += float(MeasureText("_", fontSize)) * .5f;
            if (int(GetTime() * 2) % 2 == 0) {
                DrawRectangle(int(cursorPosition.x), int(cursorPosition.y), 2, fontSize, WHITE);
            }
        }
    }
    return isMouseOverUI;
}
