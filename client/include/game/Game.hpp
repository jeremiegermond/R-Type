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

    void movePlayer();

    void updatePlayer();

    void updateEntity(Engine::EntityId id);

    void drawEntity(Engine::EntityId id, Vector3 offset = Vector3Zero());

    int addLight(Vector3 position = Vector3Zero(), Color color = WHITE, float intensity = .2);

    void updateLights();

    void addBullet(Vector3 position, Vector3 velocity);

  private:
    std::shared_ptr<ECSManager> _ecsManager;
    std::shared_ptr<UdpClient> _udpClient;
    EntityMap _gameEntities;
    std::vector<Engine::EntityId> _bullets;

    ObjectArchetype *_pObjectArchetype;
    int _playerId;
    std::vector<Light> _lights;
    std::set<int> _lightIds;
    Camera3D _camera;
};
