/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** engine.cpp by thibb1
*/

#include "engine.hpp"

Engine::Engine() : _playerId(1) {
    _camera.position = {0, 0, 30};           // Camera position
    _camera.target = Vector3Zero();          // Camera looking at point
    _camera.up = {0, 1, 0};                  // Camera up vector (rotation towards target)
    _camera.fovy = 20;                       // Camera field-of-view Y
    _camera.projection = CAMERA_PERSPECTIVE; // Camera mode type
    // SetCameraMode(_camera, CAMERA_ORBITAL);  // Set an orbital camera mode
    _companionCube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    _musicSheduled.first = 0;
    _musicVolume = 1;
    _soundVolume = 1;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        _availableLights.insert(i);
    }
}

Engine::Engine(const std::string &assetsPath) : Engine() { loadAssets(assetsPath); }

Engine::~Engine() {
    SetTraceLogLevel(LOG_INFO);
    TraceLog(LOG_INFO, "Unloading assets...");
    unloadAssets();
    TraceLog(LOG_INFO, "Assets unloaded");
    delete _udpClient;
}

void Engine::loadAssets(const std::string &assetsPath) {
    // Show loading screen
    BeginDrawing();
    ClearBackground(BLACK);
    drawText("Loading...", 24, Vector2{float(GetScreenWidth() / 2), float(GetScreenHeight() / 2)}, WHITE, true);
    EndDrawing();
    // open json file
    std::ifstream f(assetsPath);
    json data = json::parse(f);
    TraceLog(LOG_INFO, "Loading assets...");
    loadShaders(data);
    loadTextures(data);
    loadMusics(data);
    loadSounds(data);
    loadObjects(data);
    f.close();
    TraceLog(LOG_INFO, "Assets loaded");
    SetTraceLogLevel(LOG_NONE);

    float lightPos[4] = {.1, .1, .1, 1};
    int ambientLoc = GetShaderLocation(_shaders["lighting"], "ambient");
    SetShaderValue(_shaders["lighting"], ambientLoc, lightPos, SHADER_UNIFORM_VEC4);
}

void Engine::loadShaders(json data) {
    TraceLog(LOG_INFO, "Loading shaders...");
    for (auto &shader : data["shaders"]) {
        std::string name = shader["name"];
        std::string fragmentPath = shader["path_fragment"];
        Shader s;
        if (shader.contains("path_vertex")) {
            std::string vertexPath = shader["path_vertex"];
            s = LoadShader(vertexPath.c_str(), fragmentPath.c_str());
        } else {
            s = LoadShader(nullptr, fragmentPath.c_str());
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
        _shaders[name] = s;
    }
    TraceLog(LOG_INFO, "Loaded %d shaders", _shaders.size());
}

void Engine::loadTextures(json data) {
    TraceLog(LOG_INFO, "Loading textures...");
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
        _textures[name] = myTexture;
    }
    TraceLog(LOG_INFO, "Loaded %d textures", _textures.size());
}

void Engine::loadMusics(json data) {
    TraceLog(LOG_INFO, "Loading musics...");
    for (auto &music : data["musics"]) {
        MyMusic myMusic{};
        std::string name = music["name"];
        std::string path = music["path"];
        if (music.contains("index")) {
            myMusic.index = music["index"];
        }
        myMusic.name = name;
        myMusic.music = LoadMusicStream(path.c_str());
        myMusic.music.looping = true;
        _musics[name] = myMusic;
    }
    TraceLog(LOG_INFO, "Loaded %d musics", _musics.size());
}

void Engine::loadSounds(json data) {
    TraceLog(LOG_INFO, "Loading sounds...");
    for (auto &sound : data["sounds"]) {
        std::string name = sound["name"];
        std::string path = sound["path"];
        MySound mySound;
        mySound.volume = 1;
        mySound.sound = LoadSound(path.c_str());
        if (sound.contains("volume")) {
            mySound.volume = sound["volume"];
            SetSoundVolume(mySound.sound, sound["volume"]);
        }
        _sounds[name] = mySound;
    }
    TraceLog(LOG_INFO, "Loaded %d sounds", _sounds.size());
}

void Engine::loadObjects(json data) {
    TraceLog(LOG_INFO, "Loading objects...");
    for (auto &object : data["models"]) {
        std::string name = object["name"];
        std::string path = object["path"];
        auto gameObject = new GameObject(path);
        // entity _position / rot / scale at 0
        if (object.contains("scale")) {
            gameObject->setScale(object["scale"]);
            // set scale
        }
        if (object.contains("rotation") && object["rotation"].size() == 3) {
            gameObject->setRotation({object["rotation"][0], object["rotation"][1], object["rotation"][2]});
            gameObject->setRotationGoal(gameObject->getRotation());
            // set rotation
        }
        if (object.contains("position") && object["position"].size() == 3) {
            gameObject->setPosition({object["position"][0], object["position"][1], object["position"][2]});
            // set position
        }
        if (object.contains("animation")) {
            std::string animationPath = object["animation"]["path"];
            gameObject->setAnimations(animationPath);
            // add animation component and set animation
        }
        if (object.contains("textures")) {
            for (auto &texture : object["textures"]) {
                std::string type = texture["type"];
                std::string pathTexture = texture["path"];
                int materialIndex = -1;
                if (texture.contains("materialIndex"))
                    materialIndex = texture["materialIndex"];
                TraceLog(LOG_INFO, "Loading texture %s", pathTexture.c_str());
                if (type == "diffuse") {
                    gameObject->setTexture(pathTexture, MATERIAL_MAP_DIFFUSE, materialIndex);
                } else if (type == "normal") {
                    gameObject->setTexture(pathTexture, MATERIAL_MAP_NORMAL, materialIndex);
                } else if (type == "metallic") {
                    gameObject->setTexture(pathTexture, MATERIAL_MAP_METALNESS, materialIndex);
                }
            }
        }
        if (object.contains("tags")) {
            for (auto &tag : object["tags"]) {
                gameObject->addTag(tag);
            }
        }
        gameObject->setShader(_shaders["lighting"]);
        _objects[name] = gameObject;
        //_objects[name]->setShader(_shaders["lighting"]);
    }
    TraceLog(LOG_INFO, "Loaded %d objects", _objects.size());
}

void Engine::unloadAssets() {
    UnloadModel(_companionCube);
    clearSliders();
    clearButtons();
    clearEnemies();
    _bullets.clear();
    _particles2D.clear();
    _particles.clear();
    _lights.clear();
    for (auto &object : _objects) {
        delete object.second;
    }
    _objects.clear();
    for (auto &music : _musics) {
        UnloadMusicStream(music.second.music);
    }
    _musics.clear();
    for (auto &sound : _sounds) {
        UnloadSound(sound.second.sound);
    }
    _sounds.clear();
    for (auto &texture : _textures) {
        UnloadTexture(texture.second.texture);
    }
    _textures.clear();
    for (auto &shader : _shaders) {
        UnloadShader(shader.second);
    }
    _shaders.clear();
}

void Engine::clearSliders() {
    for (auto &slider : _sliders) {
        delete slider.second;
    }
    _sliders.clear();
}

void Engine::clearSlider(const std::string &name) {
    if (_sliders.find(name) != _sliders.end()) {
        delete _sliders[name];
        _sliders.erase(name);
    }
}

void Engine::clearButtons() {
    for (auto &button : _buttons) {
        delete button.second;
    }
    _buttons.clear();
}

void Engine::clearEnemies() {
    for (auto &enemy : _enemies) {
        delete enemy.second;
    }
    _enemies.clear();
}

bool Engine::updateSliders() {
    bool selected = false;
    for (auto &slider : _sliders) {
        selected = slider.second->UpdateSlider();
        if (selected)
            break;
    }
    return selected;
}

bool Engine::updateButton(const std::string &name) {
    if (_buttons.find(name) != _buttons.end()) {
        return _buttons[name]->UpdateButton();
    }
    return false;
}

void Engine::updateMusic() {
    if (IsMusicStreamPlaying(_musics[_musicPlaying].music)) {
        UpdateMusicStream(_musics[_musicPlaying].music);
    }
    if (_musicSheduled.first > 0) {
        _musicSheduled.first -= GetFrameTime();
        if (_musicSheduled.first <= 0) {
            playMusic(_musicSheduled.second);
        }
    }
}

void Engine::updateParticles() {
    for (auto it = _particles.begin(); it != _particles.end();) {
        it->scale -= .01;
        if (it->scale <= 0) {
            it = _particles.erase(it);
        } else {
            ++it;
        }
    }
}

void Engine::updateLights() {
    for (auto &light : _lights) {
        light.UpdateLightValues(_shaders["lighting"]);
    }
    // erase lights that are not active
    for (auto it = _lights.begin(); it != _lights.end();) {
        if (!it->isEnabled()) {
            _availableLights.insert(it->getIndex());
            it = _lights.erase(it);
        } else {
            ++it;
        }
    }
}

void Engine::updateObjects() {
    for (auto &object : _objects) {
        object.second->update();
    }
}

void Engine::updateCamera() { UpdateCamera(&_camera); }

void Engine::updateShaderLocView(const std::string &shaderName) {
    SetShaderValue(_shaders[shaderName], _shaders[shaderName].locs[SHADER_LOC_VECTOR_VIEW], &_camera.position, SHADER_UNIFORM_VEC3);
}

void Engine::updateParticles2D() {
    for (auto it = _particles2D.begin(); it != _particles2D.end();) {
        it->Update();
        if (!it->IsEnabled()) {
            it = _particles2D.erase(it);
        } else {
            ++it;
        }
    }
}

void Engine::updateBullets() {
    for (auto it = _bullets.begin(); it != _bullets.end();) {
        bool isColliding = false;
        it->update();
        int lightIndex = it->GetLightIndex();
        if (!isInScreen(it->GetPosition(), 10)) {
            getLight(lightIndex)->setEnabled(false);
            it = _bullets.erase(it);
            continue;
        }
        for (auto &enemy : _enemies) {
            if (it->isColliding(enemy.second->getBounds())) {
                enemy.second->takeDamage(1);
                Vector3 position = enemy.second->getPosition();
                position.z += 1;
                if (!enemy.second->isDead())
                    addParticle2D("explosion", position, float(sin(GetTime() * 10) * 90));
                getLight(lightIndex)->setEnabled(false);
                playSound("enemy_bomb");
                SetWindowTitle("Boooooooooooooom !");
                it = _bullets.erase(it);
                isColliding = true;
                break;
            }
        }
        if (!isColliding && it != _bullets.end()) {
            getLight(lightIndex)->setPosition(it->GetPosition());
            if (_particles.size() < 4500)
                addParticle(it->GetPosition(), it->getSize(), it->getColor());
            ++it;
        }
    }
}

void Engine::updateUdpClient() {
    // std::cout << "update UDP client" << std::endl;
    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
        if (isMatch(msg, "id:[0-9]+")) {
            _playerId = std::stoi(getMatch(msg, "id:([0-9]+)", 1));
            std::cout << "ID: " << _playerId << std::endl;
            auto ship = getPlayerShip();
            ship->setPosition({0, 0, 0});
        } else if (isMatch(msg, "new:[0-9]+," NB_R "," NB_R)) {
            auto match = getMatches(msg, "new:([0-9]+)," NB_R "," NB_R);
            auto newShipId = std::stoi(match[1]);
            auto newShipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            std::cout << "New ship: " << newShipId << " at " << newShipPosition.x << " " << newShipPosition.y << std::endl;
            auto ship = getObject("R9A" + match[1]);
            ship->setPosition(newShipPosition);
        } else if (isMatch(msg, "move:[0-9]+," NB_R "," NB_R)) {
            auto match = getMatches(msg, "move:([0-9]+)," NB_R "," NB_R);
            auto shipId = match[1];
            auto shipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            std::cout << "Move ship: " << shipId << " at " << shipPosition.x << " " << shipPosition.y << std::endl;
            auto ship = getObject("R9A" + shipId);
            ship->setPosition(shipPosition);
        } else if (isMatch(msg, "del:[0-9]+")) {
            auto deletedShipId = getMatch(msg, "del:([0-9]+)", 1);
            std::cout << "Delete ship: " << deletedShipId << std::endl;
            auto ship = getObject("R9A" + deletedShipId);
            ship->setPosition({-100, -100, 0});
        } else if (isMatch(msg, "shoot:[0-9]+")) {
            auto id = getMatch(msg, "shoot:([0-9]+)", 1);
            std::cout << "Shoot ship: " << id << std::endl;
            auto ship = getObject("R9A" + id);
            auto bulletPosition = ship->getPosition();
            bulletPosition.x += 1;
            auto bulletVelocity = Vector3Zero();
            bulletVelocity.x = 5;
            addBullet(bulletPosition, bulletVelocity);
        } else if (isMatch(msg, "spawn:[0-9]+," NB_R "," NB_R "," NB_R "," NB_R ",[0-9]+")) {
            auto match = getMatches(msg, "spawn:([0-9]+)," NB_R "," NB_R "," NB_R "," NB_R ",([0-9]+)");
            auto newEnemyId = std::stoi(match[1]);
            auto newEnemyPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto newEnemyVelocity = Vector3{std::stof(match[4]), std::stof(match[5]), 0};
            auto newEnemyLife = std::stoi(match[6]);
            std::cout << "New enemy: " << newEnemyId << " at " << newEnemyPosition.x << " " << newEnemyPosition.y << std::endl;
            addEnemy(newEnemyId, newEnemyPosition, newEnemyVelocity, newEnemyLife);
        } else if (isMatch(msg, "dead:[0-9]+")) {
            auto id = std::stoi(getMatch(msg, "dead:([0-9]+)", 1));
            std::cout << "Dead enemy: " << id << std::endl;
            if (_enemies.find(id) == _enemies.end())
                return;
            auto enemy = _enemies[id];
            auto enemyPosition = enemy->getPosition();
            enemyPosition.z += 1.5;
            addParticle2D("explosion", enemyPosition, float(sin(GetTime() * 10) * 90), 4);
            // erase pointer
            delete enemy;
            _enemies.erase(id);
        }
    }
}

void Engine::updateEnemies() {
    for (auto &enemy : _enemies) {
        enemy.second->update();
    }
}

void Engine::drawObject(const std::string &name, Vector3 offset) {
    if (_objects.find(name) != _objects.end() && isInScreen(_objects[name]->getPosition())) {
        _objects[name]->draw(offset);
    }
}

void Engine::drawObjects() {
    for (auto &object : _objects) {
        if (isInScreen(object.second->getPosition()))
            object.second->draw();
    }
}

void Engine::drawParticles() {
    for (auto &particle : _particles) {
        DrawModel(_companionCube, particle.position, particle.scale, particle.color);
    }
}

void Engine::drawBullets() {
    for (auto &bullet : _bullets) {
        bullet.Draw(_companionCube);
    }
}

void Engine::drawLightsSpheres() {
    for (auto &light : _lights) {
        light.DrawSphere();
    }
}

void Engine::drawParticles2D() {
    for (auto &particle : _particles2D) {
        particle.Draw(_camera);
    }
}

void Engine::drawSliders() {
    for (auto &slider : _sliders) {
        slider.second->Draw();
    }
}

void Engine::drawButtons() {
    for (auto &button : _buttons) {
        button.second->Draw();
    }
}

void Engine::drawEnemies() {
    for (auto &enemy : _enemies) {
        enemy.second->draw();
    }
}

void Engine::drawText(const std::string &text, int fontSize, Vector2 position, Color color, bool centered) {
    if (centered) {
        position.x -= MeasureText(text.c_str(), fontSize) / 2;
    }
    DrawText(text.c_str(), position.x, position.y, fontSize, color);
}

void Engine::playMusic(const std::string &name, float delay) {
    if (_musics.find(name) != _musics.end()) {
        if (delay > 0) {
            _musicSheduled.first = delay;
            _musicSheduled.second = name;
        } else {
            if (_musics.find(_musicPlaying) != _musics.end() && _musics[_musicPlaying].index != _musics[name].index) {
                StopMusicStream(_musics[_musicPlaying].music);
            }
            SetMusicVolume(_musics[name].music, _musicVolume);
            PlayMusicStream(_musics[name].music);
            _musicPlaying = name;
        }
    }
}

void Engine::playMusic(int index, float delay) {
    for (auto &music : _musics) {
        if (music.second.index == index) {
            playMusic(music.first, delay);
            break;
        }
    }
}

void Engine::playSound(const std::string &name) {
    if (_sounds.find(name) != _sounds.end()) {
        PlaySoundMulti(_sounds[name].sound);
    }
}

void Engine::addSlider(const std::string &name, Rectangle bounds, float *value, float minValue, float maxValue, bool enabled) {
    if (_sliders.find(name) != _sliders.end())
        return;
    _sliders[name] = new Slider(name, bounds, value, minValue, maxValue, enabled);
}

void Engine::addButton(const std::string &name, const std::string &text, Rectangle bounds, bool enabled) {
    if (_buttons.find(name) != _buttons.end())
        return;
    _buttons[name] = new Button(text, bounds, enabled);
}

int Engine::addLight(Vector3 position, float intensity, Color color) {
    int idx = *_availableLights.begin();
    if (idx == -1)
        return -1;
    Light light(idx, LIGHT_POINT, position, Vector3Zero(), color, _shaders["lighting"], intensity);
    _lights.emplace_back(light);
    _availableLights.erase(idx);
    return idx;
}

void Engine::addBullet(Vector3 position, Vector3 velocity) {
    unsigned char r = GetRandomValue(0, 255);
    unsigned char g = GetRandomValue(0, 255);
    unsigned char b = GetRandomValue(0, 255);
    Color color = {r, g, b, 255};
    int idx = addLight(position, .01, color);
    playSound("shot");
    _bullets.emplace_back(position, velocity, .11, idx, color);
}

void Engine::addParticle2D(const std::string &textureName, Vector3 position, float rotation, float scale) {
    if (_textures.find(textureName) == _textures.end())
        return;
    auto texture = &_textures[textureName];
    auto scaleVector = scale < 0 ? texture->scale : Vector2{scale, scale};
    //_particles2D.emplace_back(texture, position, scaleVector, rotation);
    _particles2D.insert(_particles2D.begin(), Particle2D(texture, position, scaleVector, rotation));
}

void Engine::addParticle(Vector3 position, float scale, Color color) { _particles.emplace_back(Particle{position, scale, color}); }

void Engine::addEnemy(int id, Vector3 position, Vector3 velocity, int hp) {
    if (_enemies.find(id) != _enemies.end()) {
        _enemies[id]->setPosition(position);
        _enemies[id]->setVelocity(velocity);
        _enemies[id]->setHp(hp);
    } else {
        _enemies[id] = new Enemy(position, velocity, hp, getObject("E002"));
    }
}

GameObject *Engine::getObject(const std::string &name) {
    if (_objects.find(name) != _objects.end()) {
        return _objects[name];
    }
    return nullptr;
}

GameObject *Engine::getPlayerShip() {
    auto name = "R9A" + std::to_string(_playerId);
    return getObject(name);
}

std::unordered_map<std::string, GameObject *> Engine::getObjects() { return _objects; }

Light *Engine::getLight(int index) {
    for (auto &light : _lights) {
        if (light.getIndex() == index)
            return &light;
    }
    return nullptr;
}

Camera3D *Engine::getCamera() { return &_camera; }

Button *Engine::getButton(const std::string &name) {
    if (_buttons.find(name) != _buttons.end()) {
        return _buttons[name];
    }
    return nullptr;
}

Slider *Engine::getSlider(const std::string &name) {
    if (_sliders.find(name) != _sliders.end()) {
        return _sliders[name];
    }
    return nullptr;
}

UdpClient *Engine::getUdpClient() { return _udpClient; }

void Engine::setShaderMode(const std::string &name) {
    if (_shaders.find(name) != _shaders.end()) {
        BeginShaderMode(_shaders[name]);
    }
}

void Engine::setShaderObject(const std::string &name, const std::string &shader) {
    if (_objects.find(name) != _objects.end() && _shaders.find(shader) != _shaders.end()) {
        _objects[name]->setShader(_shaders[shader]);
    }
}

void Engine::setMusicVolume(float volume) {
    _musicVolume = volume;
    if (_musics.find(_musicPlaying) != _musics.end())
        SetMusicVolume(_musics[_musicPlaying].music, _musicVolume);
}

void Engine::setSoundVolume(float volume) {
    _soundVolume = volume;
    for (auto &sound : _sounds) {
        SetSoundVolume(sound.second.sound, _soundVolume * sound.second.volume);
    }
}

void Engine::setPause(bool pause) {
    SetWindowTitle(pause ? "Paused" : "it rayworks !");
    playMusic(pause ? 2 : 1, .5);
    getButton("resume_button")->SetEnabled(pause);
    getButton("quit_button")->SetEnabled(pause);
    getSlider("music_volume")->SetEnabled(pause);
    getSlider("sound_volume")->SetEnabled(pause);
    if (pause) {
        getSlider("posX")->SetEnabled(false);
        getSlider("posY")->SetEnabled(false);
        getSlider("posZ")->SetEnabled(false);
        getSlider("rotX")->SetEnabled(false);
        getSlider("rotY")->SetEnabled(false);
        getSlider("rotZ")->SetEnabled(false);
        getSlider("scale")->SetEnabled(false);
    }
    playSound("decision");
}

void Engine::setUdpClient(UdpClient *udpClient) { _udpClient = udpClient; }

bool Engine::isInScreen(Vector3 position, float offset) {
    Vector2 screenPosition = GetWorldToScreen(position, _camera);
    return screenPosition.x > -offset && screenPosition.x < float(GetScreenWidth()) + offset && screenPosition.y > -offset &&
           screenPosition.y < float(GetScreenHeight()) + offset;
}

float *Engine::getMusicVolume() { return &_musicVolume; }

float *Engine::getSoundVolume() { return &_soundVolume; }
