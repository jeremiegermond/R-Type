/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** UpdServer.cpp by thibb1
*/

#include "network.hpp"

void UdpServer::start() {
    std::cout << "UDP server started on port 12345" << std::endl;
    srand(time(nullptr));
    _timer = std::thread([this]() {
        while (true) {
            std::this_thread::sleep_for(seconds(3));
            checkAlive();
        }
    });
    _timer.detach();
    _simulation = std::thread([this]() {
        auto startTime = system_clock::now();
        _enemySpawnTimer = startTime;
        _lastFrame = startTime;
        InitWindow(800, 450, "R-Type Server");
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            simulate();
            EndDrawing();
        }
        _stopServer = true;
        CloseWindow();
    });
    _simulation.detach();
    receiveRequest();
    _io_context.run();
}

UdpServer::~UdpServer() {
    std::cout << "UDP server stopped" << std::endl;
    _socket.cancel();
    _socket.close();
}

void UdpServer::handleRequest(std::error_code ec, std::size_t bytes_recvd) {
    if (!ec && bytes_recvd > 0) {
        std::string msg(_buffer.begin(), _buffer.begin() + bytes_recvd);
        std::cout << "From: " << _sender_endpoint << " ";
        std::cout << "Received: " << msg << std::endl;
        if (_clients.find(_sender_endpoint) == _clients.end()) {
            if (_ids.empty()) {
                sendResponse(_sender_endpoint, "full");
                return;
            }
            auto id = *_ids.begin();
            _ids.erase(id);
            _clients[_sender_endpoint] = Player(id);
            sendResponse(_sender_endpoint, "id:" + std::to_string(_clients[_sender_endpoint].getId()));
            sendAll("new:" + std::to_string(_clients[_sender_endpoint].getId()) + "," + vectorToString(_clients[_sender_endpoint].getPosition()),
                    false);
            // send all players to new player
            for (auto &client : _clients) {
                if (client.first != _sender_endpoint) {
                    sendResponse(_sender_endpoint,
                                 "new:" + std::to_string(client.second.getId()) + "," + vectorToString(client.second.getPosition()));
                }
            }
            // send all enemies to new player
            for (auto &enemy : _enemies) {
                sendResponse(_sender_endpoint, "spawn:" + std::to_string(enemy.getId()) + "," + vectorToString(enemy.getPosition()) + "," +
                                                   vectorToString(enemy.getVelocity()) + "," + std::to_string(enemy.getHp()));
            }
        }
        auto &player = _clients[_sender_endpoint];
        std::cout << "Player: " << player.getId() << std::endl;
        if (msg == "ping") {
            sendResponse(_sender_endpoint, "pong");
            player.setAlive(true);
        } else if (msg == "disconnect") {
            removeClient(_sender_endpoint);
        } else if (isMatch(msg, "^move:(left|right|up|down)," NB_R "," NB_R "$")) {
            auto match = getMatches(msg, "^move:(left|right|up|down)," NB_R "," NB_R "$");
            auto direction = match[1];
            auto x = std::stof(match[2]);
            auto y = std::stof(match[3]);
            auto pos = Vector2{x, y};
            auto valid = player.move(direction, pos);
            sendAll("move:" + std::to_string(player.getId()) + "," + vectorToString(player.getPosition()), !valid);
        } else if (msg == "shoot") {
            auto bulletPosition = player.getPosition();
            bulletPosition.x += 0.5;
            auto bulletVelocity = Vector2{5, 0};
            _bullets.emplace_back(bulletPosition, bulletVelocity, .1, player.getId());
            sendAll("shoot:" + std::to_string(player.getId()), false);
        }
    }
}

void UdpServer::sendResponse(const udp::endpoint &endpoint, const std::string &msg) {
    std::lock_guard<std::mutex> lock(_socketMutex);
    _socket.send_to(asio::buffer(msg), endpoint);
}

void UdpServer::receiveRequest() {
    if (_stopServer)
        return;
    _socket.async_receive_from(asio::buffer(_buffer), _sender_endpoint, [this](std::error_code ec, std::size_t bytes_recvd) {
        handleRequest(ec, bytes_recvd);
        receiveRequest();
    });
}

void UdpServer::checkAlive() {
    if (_clients.empty())
        return;
    std::cout << "Checking clients..." << std::endl;
    auto it = _clients.begin();
    while (it != _clients.end()) {
        if (!it->second.isAlive()) {
            std::cout << "Client: " << it->first << " is dead" << std::endl;
            removeClient(it->first, false);
            it = _clients.erase(it);
        } else {
            it->second.setAlive(false);
            ++it;
        }
    }
}

void UdpServer::sendAll(const std::string &msg, bool includeSender) {
    for (auto &client : _clients) {
        if (includeSender || client.first != _sender_endpoint)
            sendResponse(client.first, msg);
    }
}

void UdpServer::removeClient(const udp::endpoint &endpoint, bool erase) {
    if (_clients.find(endpoint) != _clients.end()) {
        _ids.insert(_clients[endpoint].getId());
        sendAll("del:" + std::to_string(_clients[endpoint].getId()), true);
        if (erase)
            _clients.erase(endpoint);
    }
}

void UdpServer::simulate() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // update frame time
    auto tp = high_resolution_clock::now();
    auto deltaTime = float(duration_cast<milliseconds>(tp - _lastFrame).count()) / 1000.0f;
    _lastFrame = tp;
    // std::cout << "Delta time: " << deltaTime << std::endl;
    //  update enemies
    for (auto enemy = _enemies.begin(); enemy != _enemies.end();) {
        enemy->update(deltaTime);
        auto enemyPosition = enemy->getPosition();
        DrawText("x", (enemyPosition.x + 10) * 40, (-enemyPosition.y + 5) * 40, 20, RED);
        // std::cout << "Enemy: " << enemy->getId() << " " << enemyPosition.x << " " << enemyPosition.y << std::endl;
        if (enemyPosition.x < -10) {
            std::cout << "Enemy: " << enemy->getId() << " out of bounds" << std::endl;
            _enemyIds.insert(enemy->getId());
            enemy = _enemies.erase(enemy);
        } else {
            ++enemy;
        }
    }
    // update bullets
    for (auto bullet = _bullets.begin(); bullet != _bullets.end();) {
        bullet->update(deltaTime);
        auto bulletPosition = bullet->getPosition();
        DrawText("o", (bulletPosition.x + 10) * 40, (-bulletPosition.y + 5) * 40, 20, ORANGE);
        bool bulletErased = false;
        for (auto enemy = _enemies.begin(); enemy != _enemies.end();) {
            if (bullet->isColliding(enemy->getBounds())) {
                enemy->takeDamage(bullet->getDamage());
                if (enemy->isDead()) {
                    std::cout << "Enemy: " << enemy->getId() << " was killed" << std::endl;
                    sendAll("dead:" + std::to_string(enemy->getId()));
                    _enemyIds.insert(enemy->getId());
                    enemy = _enemies.erase(enemy);
                    _enemySpawnTimer += std::chrono::seconds(1);
                } else {
                    sendAll("damaged:" + std::to_string(enemy->getId()) + "," + std::to_string(enemy->getHp()));
                    ++enemy;
                }
                bullet = _bullets.erase(bullet);
                bulletErased = true;
                break;
            } else {
                ++enemy;
            }
        }
        if (bulletErased) {
            continue;
        }
        if (bullet->isOutOfBounds()) {
            bullet = _bullets.erase(bullet);
        } else {
            ++bullet;
        }
    }
    if (!_enemyIds.empty()) {
        auto now = std::chrono::system_clock::now();
        if (now - _enemySpawnTimer > std::chrono::seconds(2)) {
            _enemySpawnTimer = now;
            auto id = *_enemyIds.begin();
            _enemyIds.erase(id);
            auto position = Vector2{11, (float)rand() / RAND_MAX * 8 - 4};
            auto velocity = Vector2{-1, 0};
            _enemies.emplace_back(id, position, velocity, 2);
            sendAll("spawn:" + std::to_string(id) + "," + vectorToString(position) + "," + vectorToString(velocity) + ",2");
        }
    }
    for (auto &client : _clients) {
        auto player = client.second;
        auto position = player.getPosition();
        DrawText("p", (position.x + 10) * 40, (-position.y + 5) * 40, 20, GREEN);
    }
}
