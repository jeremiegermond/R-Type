/*
 * EPITECH PROJECT, 2023
 * rtype
 * File description:
 * GameBase.hpp
 */

#pragma once

#include "engine/manager/ECSManagerBase.hpp"
#include "engine/utils/Defines.hpp"

namespace Engine {
/*
 * @brief Base class for all games
 */
class GameBase {
  public:
    /**
     * @brief Construct a new Game Base object
     */
    GameBase();

    /**
     * @brief destroy the Game Base object
     */
    virtual ~GameBase();

    /**
     * @brief init the game
     */
    void init();

    /**
     * @brief update the game
     */
    void update();

    /**
     * @brief destroy the game
     */
    void destroy();

    /**
     * @brief Draw simple text centered on the screen
     * @param text
     */
    static void drawTextCentered(const std::string &text, Color color = WHITE);

  protected:
    /**
     * @brief init the game
     */
    virtual void initGame() = 0;

    /**
     * @brief update the game
     */
    virtual void updateGame() = 0;

    /**
     * @brief destroy the game
     */
    virtual void destroyGame() = 0;

    /**
     * @brief Load all assets
     * @param json assets
     * Override this function to load your own assets
     */
    virtual void loadAssetsGame(const json &assets) { UNUSED(assets); }

    /**
     * @brief Load all assets
     * @param string to a json file containing all assets paths
     */
    void loadAssets(const std::string &path);

    std::unordered_map<std::string, std::shared_ptr<CShader>> _shaders;
    std::unordered_map<std::string, std::shared_ptr<CTexture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<CSound>> _sounds;
    std::unordered_map<std::string, std::shared_ptr<CMusic>> _musics;
    std::unordered_map<std::string, std::shared_ptr<CModel>> _models;
    std::unordered_map<std::string, std::shared_ptr<CAnimation>> _animations;

  private:
    /**
     * @brief Load all shaders
     * @param json shaders
     */
    void loadShaders(const json &shaders);

    /**
     * @brief Load all textures
     * @param json textures
     */
    void loadTextures(const json &textures);

    /**
     * @brief Load all sounds
     * @param json sounds
     */
    void loadSounds(const json &sounds);

    /**
     * @brief Load all musics
     * @param json musics
     */
    void loadMusics(const json &musics);

    /**
     * @brief Load all models
     * @param json models
     */
    void loadModels(const json &models);

    /**
     * @brief Load all animations
     * @param json animations
     */
    void loadAnimations(const json &animations);

    /**
     * @brief Unload all assets
     */
    void unloadAssets();
};
} // namespace Engine
