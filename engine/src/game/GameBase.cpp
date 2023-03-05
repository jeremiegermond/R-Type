/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameBase.cpp
*/

#include "engine/game/GameBase.hpp"

#include "raylib.h"

namespace Engine {

    /**
     * GameBase::GameBase() = default;
     */
    GameBase::GameBase() = default;

    /**
     * A destructor for the GameBase class.
     */
    GameBase::~GameBase() = default;

    /**
     * It initializes the window, the audio device, the target FPS, draws the text "Loading...",
     * initializes the game, and sets the trace log level to none
     */
    void GameBase::init() {
        InitWindow(1070, 600, "R-Type");
        InitAudioDevice();
        SetTargetFPS(60);
        drawTextCentered("Loading...");
        initGame();
        SetTraceLogLevel(LOG_NONE);
    }

    /**
     * `update()` is a function that calls `updateGame()` while the window is open
     */
    void GameBase::update() {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            updateGame();
            EndDrawing();
        }
    }

    /**
     * It destroys the game, unloads the assets, closes the audio device, closes the window, and logs
     * the game closing
     */
    void GameBase::destroy() {
        SetTraceLogLevel(LOG_INFO);
        destroyGame();
        unloadAssets();
        if (IsAudioDeviceReady())
            CloseAudioDevice();
        CloseWindow();
        TraceLog(LOG_INFO, "Game closed");
    }

    /**
     * It loads assets from a json file
     * 
     * @param path The path to the assets file.
     */
    void GameBase::loadAssets(const std::string &path) {
        if (path.empty())
            throw std::runtime_error("Invalid path to assets file");
        if (!FileExists(path.c_str()))
            throw std::runtime_error("Assets file not found");
        std::ifstream f(path);
        json data = json::parse(f);
        if (data.contains("shaders")) {
            drawTextCentered("Loading shaders...");
            loadShaders(data["shaders"]);
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

    /**
     * It draws text centered on the screen
     * 
     * @param text The text to draw
     * @param color The color of the text.
     */
    void GameBase::drawTextCentered(const std::string &text, Color color) {
        BeginDrawing();
        ClearBackground(BLACK);
        auto textSize = MeasureText(text.c_str(), 40);
        auto x = (GetScreenWidth() - textSize) / 2;
        auto y = (GetScreenHeight() - 40) / 2;
        DrawText(text.c_str(), x, y, 40, color);
        EndDrawing();
    }

    /**
     * It loads shaders from a json file
     * 
     * @param shaders The json object containing the shaders
     */
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

    /**
     * It loads textures from a json file
     * 
     * @param textures The json object that contains the textures.
     */
    void GameBase::loadTextures(const json &textures) {
        for (auto &texture : textures) {
            if (!texture.contains("name") || !texture.contains("path"))
                continue;
            auto newTexture = std::make_shared<Engine::CTexture>();
            std::string name = texture["name"];
            std::string path = texture["path"];
            newTexture->setTexture(path);
            if (texture.contains("rows") && texture.contains("columns")) {
                newTexture->setRowsAndColumns(texture["rows"], texture["columns"]);
            }
            if (texture.contains("scale") && texture["scale"].size() == 2) {
                newTexture->setScale({texture["scale"][0], texture["scale"][1]});
            }
            _textures[name] = newTexture;
        }
    }

    /**
     * It loads sounds from a json file
     * 
     * @param sounds The json object that contains the sounds.
     */
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

    /**
     * It loads musics from a json file
     * 
     * @param musics The json object that contains the musics to load.
     */
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

    /**
     * It loads models from a json file
     * 
     * @param models The JSON object containing the models to load.
     */
    void GameBase::loadModels(const json &models) {
        for (auto &model : models) {
            if (!model.contains("name") || !model.contains("path") || _models.contains(model["name"]))
                continue;
            auto newModel = std::make_shared<Engine::CModel>();
            std::string name = model["name"];
            std::string path = model["path"];
            newModel->setModel(path);
            if (model.contains("textures")) {
                for (auto &texture : model["textures"]) {
                    if (!texture.contains("name") || !texture.contains("path"))
                        continue;
                    newModel->setModelTexture(texture["name"], texture["path"]);
                }
            }
            _models[name] = newModel;
        }
    }

    /**
     * It loads animations from a json file
     * 
     * @param animations The json object that contains the animations.
     */
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

    /**
     * It clears all the maps that hold the assets
     */
    void GameBase::unloadAssets() {
        _shaders.clear();
        _textures.clear();
        _sounds.clear();
        _musics.clear();
        _animations.clear();
        _models.clear();
    }

    /**
     * If the window is ready, and if it's fullscreen, return the monitor's width and height, otherwise
     * return the screen's width and height
     * 
     * @return A Vector2 object.
     */
    Vector2 GameBase::getWindowSize() {
        if (!IsWindowReady())
            return {0, 0};
        if (IsWindowFullscreen()) {
            auto monitor = GetCurrentMonitor();
            return {float(GetMonitorWidth(monitor)), float(GetMonitorHeight(monitor))};
        }
        return {float(GetScreenWidth()), float(GetScreenHeight())};
    }
}
