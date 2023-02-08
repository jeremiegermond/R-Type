/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Game.hpp
*/

#pragma once

#include "engine/game/GameBase.hpp"
#include "engine/utils/Defines.hpp"
#include "manager/ECSManager.hpp"
#include "network/UdpClient.hpp"

class Game : public Engine::GameBase {
  public:
    Game();

    void initGame() override;
    void updateGame() override;
    void destroyGame() override;

    void initSocket(const std::string &ip, unsigned short port);

  private:
    std::shared_ptr<ECSManager> _ecsManager;
    std::shared_ptr<UdpClient> _udpClient;
    EntityMap _gameEntities;
};
