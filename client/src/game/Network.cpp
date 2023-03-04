/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Network.cpp
*/

#include "game/Game.hpp"

void Game::updateNetwork() {
    while (_udpClient->hasMessage()) {
        auto msg = _udpClient->receive();
        std::cout << "Message received: " << msg << std::endl;
        if (Engine::Regex::isMatch(msg, "id:[0-9]+")) {
            _playerId = std::stoi(Engine::Regex::getMatch(msg, "id:([0-9]+)", 1));
            auto name = "R9A" + std::to_string(_playerId);
            auto [shipPosition, ship] = _pObjectArchetype.getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(true);
            shipPosition.setPosition(Vector3Zero());
        } else if (Engine::Regex::isMatch(msg, "new:[0-9]+," NB_R "," NB_R)) {
            auto match = Engine::Regex::getMatches(msg, "new:([0-9]+)," NB_R "," NB_R);
            auto newShipId = std::stoi(match[1]);
            auto newShipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto name = "R9A" + std::to_string(newShipId);
            auto [shipPosition, ship] = _pObjectArchetype.getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(true);
            shipPosition.setPosition(newShipPosition);
        } else if (Engine::Regex::isMatch(msg, "move:[0-9]+," NB_R "," NB_R)) {
            auto match = Engine::Regex::getMatches(msg, "move:([0-9]+)," NB_R "," NB_R);
            auto shipId = match[1];
            auto shipPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto name = "R9A" + shipId;
            auto [shipPos, ship] = _pObjectArchetype.getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            shipPos.setPosition(shipPosition);
        } else if (Engine::Regex::isMatch(msg, "del:[0-9]+")) {
            auto deletedShipId = Engine::Regex::getMatch(msg, "del:([0-9]+)", 1);
            auto name = "R9A" + deletedShipId;
            auto [shipPosition, ship] = _pObjectArchetype.getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            ship.setActive(false);
            shipPosition.setPosition({-100, -100, 0});
        } else if (Engine::Regex::isMatch(msg, "shoot:[0-9]+")) {
            auto id = Engine::Regex::getMatch(msg, "shoot:([0-9]+)", 1);
            auto name = "R9A" + id;
            auto [shipPosition, ship] = _pObjectArchetype.getComponent<Engine::CPosition, Engine::CObject>(_gameEntities[name]);
            auto bulletPosition = shipPosition.getPosition();
            bulletPosition.x += .4;
            auto bulletVelocity = Vector3Zero();
            bulletVelocity.x = 5;
            addBullet(bulletPosition, bulletVelocity);
        } else if (Engine::Regex::isMatch(msg, "spawn:[0-9]+," NB_R "," NB_R "," NB_R "," NB_R ",[0-9]+")) {
            auto match = Engine::Regex::getMatches(msg, "spawn:([0-9]+)," NB_R "," NB_R "," NB_R "," NB_R ",([0-9]+)");
            auto newEnemyId = std::stoi(match[1]);
            auto newEnemyPosition = Vector3{std::stof(match[2]), std::stof(match[3]), 0};
            auto newEnemyVelocity = Vector3{std::stof(match[4]), std::stof(match[5]), 0};
            auto newEnemyLife = std::stoi(match[6]);
            addEnemy(newEnemyId, newEnemyPosition, newEnemyVelocity, newEnemyLife);
        } else if (Engine::Regex::isMatch(msg, "dead:[0-9]+")) {
            auto id = std::stoi(Engine::Regex::getMatch(msg, "dead:([0-9]+)", 1));
            if (_enemies.find(id) == _enemies.end())
                return;
            auto enemyPos = _pObjectArchetype.getComponent<Engine::CPosition>(_enemies[id]);
            playSound("enemy_bomb");
            addAnimatedSprite("explosion", enemyPos.getPosition());
            _enemies.erase(id);
        } else if (Engine::Regex::isMatch(msg, "name:[0-9]+,[a-zA-Z0-9 ]+")) {
            auto match = Engine::Regex::getMatches(msg, "name:([0-9]+),([a-zA-Z0-9 ]+)");
            auto shipId = match[1];
            auto name = "R9A" + shipId;
            auto [shipObject, shipName] = _pObjectArchetype.getComponent<Engine::CObject, CText>(_gameEntities[name]);
            shipName.setText(match[2]);
        } else if (Engine::Regex::isMatch(msg, "damaged:[0-9]+,[0-9]+")) {
            auto match = Engine::Regex::getMatches(msg, "damaged:([0-9]+),([0-9]+)");
            auto enemyId = std::stoi(match[1]);
            auto health = std::stoi(match[2]);
            if (_enemies.find(enemyId) == _enemies.end())
                return;
            auto [enemyHealth, enemyPos] = _pObjectArchetype.getComponent<CHealth, Engine::CPosition>(_enemies[enemyId]);
            enemyHealth.setHealth(health);
            playSound("enemy_bomb");
            addAnimatedSprite("explosion", enemyPos.getPosition());
        } else if (Engine::Regex::isMatch(msg, "score:[0-9]+,[0-9]+")) {
            auto match = Engine::Regex::getMatches(msg, "score:([0-9]+),([0-9]+)");
            auto name = "score_" + match[1];
            auto score = match[2];
            if (_uiElements.contains(name) && _gameEntities.contains("R9A" + match[1])) {
                auto playerName = _pObjectArchetype.getComponent<CText>(_gameEntities["R9A" + match[1]]).getText();
                _pUIArchetype.getComponent<CText>(_uiElements[name]).setText("Player " + match[1] + ": " + score + " - " + playerName);
            }
        }
    }
}
