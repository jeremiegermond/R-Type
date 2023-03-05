/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.hpp
*/

#pragma once

#include "engine/game/GameBase.hpp"
#include "engine/utils/Defines.hpp"
#include "game/Light.hpp"
#include "manager/ECSManager.hpp"
#include "network/UdpClient.hpp"

enum GameState { MENU, GAME };

class Game : public Engine::GameBase {
  public:
    Game();

    void initGame() override;
    void updateGame() override;
    void updateNetwork();
    void destroyGame() override;

    void drawGame();

    void initSocket(const std::string &ip, unsigned short port);

    void loadEntities(const std::string &path);

    void loadUI(json &ui);

    void loadObject(json &object);

    void movePlayer();

    void updatePlayer();

    void updateEntity(Engine::EntityId id);

    void drawEntity(Engine::EntityId id, Vector3 offset = Vector3Zero());

    void drawTexture(Engine::EntityId id, Vector3 offset = Vector3Zero());

    void updateTextures();

    void addAnimatedSprite(const std::string &name, Vector3 position);

    int addLight(Vector3 position = Vector3Zero(), Color color = WHITE, float intensity = .2);

    void updateLights();

    void addBullet(Vector3 position, Vector3 velocity);

    void updateBullets();

    void addEnemy(int id, Vector3 position, Vector3 velocity, int hp);

    void playSound(const std::string &name);

    void playMusic(const std::string &name);

    void updateMusic();

    void setGameState(GameState state);

    void updateMenu();

    void updateGameplay();

    void drawUI();

    bool drawUIEntity(Engine::EntityId id);

    void loadAssetsGame(const json &assets) override;

    static ParticleEmitterSettings loadParticleEmitterSettings(const json &emitter, ParticleEmitterSettings settings = {});

    static void drawLetter3D(char letter, Vector3 position, float fontSize, Color tint = WHITE, Font font = GetFontDefault());

    static void drawText3DPro(const std::string &text, Vector3 position, float fontSize, float fontSpacing, Color tint = WHITE,
                              Font font = GetFontDefault());

    static void drawText3D(const std::string &text, Vector3 position, float fontSize, Color tint = WHITE);

    static Vector3 measureText3D(const std::string &text, float fontSize, Font font = GetFontDefault());

    void getRooms();

    void addRoom(const std::string &room);

  private:
    /**
     * The current state of the game.
     * Can be either GameState::MENU or GameState::GAME
     */
    GameState _gameState;

    /**
     * The ECS manager.
     * Used to manage Archetypes
     */
    std::shared_ptr<ECSManager> _ecsManager;

    /**
     * The UDP client.
     * Used to send and receive data from the server
     */
    std::shared_ptr<UdpClient> _udpClient;

    /**
     * Variables for the game loop
     */
    EntityMap _gameEntities;
    std::vector<Engine::EntityId> _bullets;
    std::unordered_map<int, Engine::EntityId> _enemies;
    std::vector<Engine::EntityId> _animatedSprites;
    int _playerId;
    Engine::EntityId _camera;
    std::string _currentMusic;
    std::vector<Engine::EntityId> _rooms;
    bool _gameOver;
    unsigned short _selectedRoom;

    /**
     * Various Archetypes used by the game to create and access entities
     */
    ObjectArchetype _pObjectArchetype;
    CameraArchetype _pCameraArchetype;
    SpriteArchetype _pSpriteArchetype;
    UIArchetype _pUIArchetype;

    /**
     * The list of lights in the game
     */
    std::vector<Light> _lights;
    std::set<int> _lightIds;

    /**
     * UI elements
     */
    EntityMap _uiElements;
    std::string _playerName;

    /**
     * Emitters used by the game
     */
    std::unordered_map<std::string, CParticleEmitter> _emitters;
};
