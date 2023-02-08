/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameBase.cpp
*/

#include "engine/game/GameBase.hpp"

#include "raylib.h"

namespace Engine {

    GameBase::GameBase() = default;

    GameBase::~GameBase() = default;

    void GameBase::init() {
        InitWindow(800, 450, "R-Type");
        InitAudioDevice();
        SetTargetFPS(60);
        drawTextCentered("Loading...");
        initGame();
        SetTraceLogLevel(LOG_NONE);
    }

    void GameBase::update() {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            updateGame();
            EndDrawing();
        }
    }

    void GameBase::destroy() {
        SetTraceLogLevel(LOG_INFO);
        destroyGame();
        unloadAssets();
        if (IsAudioDeviceReady())
            CloseAudioDevice();
        CloseWindow();
        TraceLog(LOG_INFO, "Game closed");
    }

    void GameBase::loadAssets(const std::string &path) {
        if (path.empty())
            throw std::runtime_error("Invalid path to assets file");
        if (!FileExists(path.c_str()))
            throw std::runtime_error("Assets file not found");
        std::ifstream f(path);
        json data = json::parse(f);
        if (data.contains("shaders")) {
            drawTextCentered("Loading shaders...");
            // loadShaders(data["shaders"]);
        }
        if (data.contains("textures")) {
            drawTextCentered("Loading textures...");
            loadTextures(data["textures"]);
        }
        if (data.contains("musics")) {
            drawTextCentered("Loading musics...");
            loadMusics(data["musics"]);
        }
        if (data.contains("sounds")) {
            drawTextCentered("Loading sounds...");
            loadSounds(data["sounds"]);
        }
        if (data.contains("models")) {
            drawTextCentered("Loading models...");
            loadModels(data["models"]);
        }
        if (data.contains("animations")) {
            drawTextCentered("Loading animations...");
            loadAnimations(data["animations"]);
        }
        loadAssetsGame(data);
        drawTextCentered("Finished loading!", GREEN);
        f.close();
    }

    void GameBase::drawTextCentered(const std::string &text, Color color) {
        BeginDrawing();
        ClearBackground(BLACK);
        auto textSize = MeasureText(text.c_str(), 40);
        auto x = (GetScreenWidth() - textSize) / 2;
        auto y = (GetScreenHeight() - 40) / 2;
        DrawText(text.c_str(), x, y, 40, color);
        EndDrawing();
    }

    void GameBase::loadShaders(const json &shaders) {
        for (auto &shader : shaders) {
            if (!shader.contains("name") || !shader.contains("path_fragment") || _shaders.contains(shader["name"]))
                continue;
            auto newShader = std::make_shared<Engine::CShader>();
            std::string name = shader["name"];
            std::string fragmentPath = shader["path_fragment"];
            if (shader.contains("path_vertex")) {
                newShader->setShader(fragmentPath, shader["path_vertex"]);
            } else {
                newShader->setShader(fragmentPath, "");
            }
            if (shader.contains("locs")) {
                for (auto &loc : shader["locs"]) {
                    if (!loc.contains("name") || !loc.contains("from"))
                        continue;
                    newShader->setShaderLocation(loc["name"], loc["from"]);
                }
            }
            _shaders[name] = newShader;
        }
    }

    void GameBase::loadTextures(const json &textures) {
        for (auto &texture : textures) {
            if (!texture.contains("name") || !texture.contains("path") || _textures.contains(texture["name"]))
                continue;
            auto newTexture = std::make_shared<Engine::CTexture>();
            std::string name = texture["name"];
            std::string path = texture["path"];
            newTexture->setTexture(path);
            _textures[name] = newTexture;
        }
    }

    void GameBase::loadSounds(const json &sounds) {
        for (auto &sound : sounds) {
            if (!sound.contains("name") || !sound.contains("path") || _sounds.contains(sound["name"]))
                continue;
            auto newSound = std::make_shared<Engine::CSound>();
            std::string name = sound["name"];
            std::string path = sound["path"];
            newSound->setSound(path);
            _sounds[name] = newSound;
        }
    }

    void GameBase::loadMusics(const json &musics) {
        for (auto &music : musics) {
            if (!music.contains("name") || !music.contains("path") || _musics.contains(music["name"]))
                continue;
            auto newMusic = std::make_shared<Engine::CMusic>();
            std::string name = music["name"];
            std::string path = music["path"];
            newMusic->setMusic(path);
            newMusic->setLoop(true);
            _musics[name] = newMusic;
        }
    }

    void GameBase::loadModels(const json &models) {
        for (auto &model : models) {
            if (!model.contains("name") || !model.contains("path") || _models.contains(model["name"]))
                continue;
            auto newModel = std::make_shared<Engine::CModel>();
            std::string name = model["name"];
            std::string path = model["path"];
            newModel->setModel(path);
            _models[name] = newModel;
        }
    }

    void GameBase::loadAnimations(const json &animations) {
        for (auto &animation : animations) {
            if (!animation.contains("name") || !animation.contains("path") || _animations.contains(animation["name"]))
                continue;
            auto newAnimation = std::make_shared<Engine::CAnimation>();
            std::string name = animation["name"];
            std::string path = animation["path"];
            newAnimation->setAnimation(path);
            _animations[name] = newAnimation;
        }
    }

    void GameBase::unloadAssets() {
        _shaders.clear();
        _textures.clear();
        _sounds.clear();
        _musics.clear();
        _models.clear();
        _animations.clear();
    }
}
