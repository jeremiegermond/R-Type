/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** engine.cpp by thibb1
*/

#include "engine.hpp"

Engine::Engine(const std::string &assetsPath) {
    _camera.position = {0, 0, 30};           // Camera position
    _camera.target = Vector3Zero();          // Camera looking at point
    _camera.up = {0, 1, 0};                  // Camera up vector (rotation towards target)
    _camera.fovy = 20;                       // Camera field-of-view Y
    _camera.projection = CAMERA_PERSPECTIVE; // Camera mode type
    // SetCameraMode(_camera, CAMERA_ORBITAL);  // Set an orbital camera mode
    _companionCube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    loadAssets(assetsPath);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        _availableLights.insert(i);
    }
}

Engine::~Engine() {
    SetTraceLogLevel(LOG_INFO);
    TraceLog(LOG_INFO, "Unloading assets...");
    unloadAssets();
    TraceLog(LOG_INFO, "Assets unloaded");
}

void Engine::loadAssets(const std::string &assetsPath) {
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
        MyMusic myMusic;
        std::string name = music["name"];
        std::string path = music["path"];
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
        _sounds[name] = LoadSound(path.c_str());
    }
    TraceLog(LOG_INFO, "Loaded %d sounds", _sounds.size());
}

void Engine::loadObjects(json data) {
    TraceLog(LOG_INFO, "Loading objects...");
    for (auto &object : data["models"]) {
        std::string name = object["name"];
        std::string path = object["path"];
        auto gameObject = new GameObject(path);
        if (object.contains("scale")) {
            gameObject->SetScale(object["scale"]);
        }
        if (object.contains("rotation") && object["rotation"].size() == 3) {
            gameObject->SetRotation({object["rotation"][0], object["rotation"][1], object["rotation"][2]});
            gameObject->SetRotationGoal(gameObject->GetRotation());
        }
        if (object.contains("position") && object["position"].size() == 3) {
            gameObject->SetPosition({object["position"][0], object["position"][1], object["position"][2]});
        }
        if (object.contains("animation")) {
            std::string animationPath = object["animation"]["path"];
            gameObject->SetAnimations(animationPath);
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
                    gameObject->SetTexture(pathTexture, MATERIAL_MAP_DIFFUSE, materialIndex);
                } else if (type == "normal") {
                    gameObject->SetTexture(pathTexture, MATERIAL_MAP_NORMAL, materialIndex);
                } else if (type == "metallic") {
                    gameObject->SetTexture(pathTexture, MATERIAL_MAP_METALNESS, materialIndex);
                }
            }
        }
        if (object.contains("tags")) {
            for (auto &tag : object["tags"]) {
                gameObject->SetTag(tag);
            }
        }
        gameObject->SetShader(_shaders["lighting"]);
        _objects[name] = gameObject;
        //_objects[name]->SetShader(_shaders["lighting"]);
    }
    TraceLog(LOG_INFO, "Loaded %d objects", _objects.size());
}

void Engine::unloadAssets() {
    UnloadModel(_companionCube);
    clearSliders();
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
        UnloadSound(sound.second);
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

void Engine::clearSliders() { _sliders.clear(); }

bool Engine::updateSliders() {
    bool selected = false;
    for (auto &slider : _sliders) {
        selected = slider.UpdateSlider();
        if (selected)
            break;
    }
    return selected;
}

void Engine::updateMusic() {
    if (IsMusicStreamPlaying(_musics[_musicPlaying].music)) {
        UpdateMusicStream(_musics[_musicPlaying].music);
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
        object.second->Update();
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
        it->Update();
        int lightIndex = it->GetLightIndex();
        if (!isInScreen(it->GetPosition(), 10)) {
            getLight(lightIndex)->setEnabled(false);
            it = _bullets.erase(it);
            continue;
        }
        for (auto &object : _objects) {
            if (object.second->IsTagged("enemy")) {
                if (it->IsColliding(object.second->GetMyObjectBoundingBox())) {
                    Vector3 position = it->GetPosition();
                    position.z += 3;
                    addParticle2D("explosion", position, float(sin(GetTime() * 10) * 90));
                    getLight(lightIndex)->setEnabled(false);
                    it = _bullets.erase(it);
                    break;
                }
            }
        }
        if (it != _bullets.end()) {
            getLight(lightIndex)->setPosition(it->GetPosition());
            if (_particles.size() < 4500)
                addParticle(it->GetPosition(), it->getSize(), it->getColor());
            ++it;
        }
    }
}

void Engine::drawObject(const std::string &name, Vector3 offset) {
    if (_objects.find(name) != _objects.end() && isInScreen(_objects[name]->GetPosition())) {
        _objects[name]->Draw(offset);
    }
}

void Engine::drawObjects() {
    for (auto &object : _objects) {
        if (isInScreen(object.second->GetPosition()))
            object.second->Draw();
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
        slider.Draw();
    }
}

void Engine::playMusic(const std::string &name) {
    if (_musics.find(name) != _musics.end()) {
        PlayMusicStream(_musics[name].music);
        _musicPlaying = name;
    }
}

void Engine::setShaderMode(const std::string &name) {
    if (_shaders.find(name) != _shaders.end()) {
        BeginShaderMode(_shaders[name]);
    }
}

void Engine::addSlider(const std::string &name, Rectangle bounds, float *value, float minValue, float maxValue) {
    _sliders.emplace_back(name, bounds, value, minValue, maxValue);
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
    _bullets.emplace_back(position, velocity, .11, idx, color);
}

void Engine::addParticle2D(const std::string &textureName, Vector3 position, float rotation) {
    if (_textures.find(textureName) == _textures.end())
        return;
    auto texture = &_textures[textureName];
    _particles2D.emplace_back(texture, position, texture->scale, rotation);
}

void Engine::addParticle(Vector3 position, float scale, Color color) { _particles.emplace_back(Particle{position, scale, color}); }

GameObject *Engine::getObject(const std::string &name) {
    if (_objects.find(name) != _objects.end()) {
        return _objects[name];
    }
    return nullptr;
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

void Engine::setShaderObject(const std::string &name, const std::string &shader) {
    if (_objects.find(name) != _objects.end() && _shaders.find(shader) != _shaders.end()) {
        _objects[name]->SetShader(_shaders[shader]);
    }
}

bool Engine::isInScreen(Vector3 position, float offset) {
    Vector2 screenPosition = GetWorldToScreen(position, _camera);
    return screenPosition.x > -offset && screenPosition.x < float(GetScreenWidth()) + offset && screenPosition.y > -offset &&
           screenPosition.y < float(GetScreenHeight()) + offset;
}
