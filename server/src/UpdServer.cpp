/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** UpdServer.cpp by thibb1
*/

#include "network.hpp"

void sigintHandler(int sig) {
    std::cout << "Stopping server..." << std::endl;
    exit(0);
}

void UdpServer::start() {
    // make ctrl+c stop the server
    signal(SIGINT, sigintHandler);
    std::cout << "UDP server started on port 12345" << std::endl;
    _timer = std::thread([this]() {
        while (true) {
            std::this_thread::sleep_for(seconds(3));
            checkAlive();
        }
    });
    _timer.detach();
    receiveRequest();
    _io_context.run();
}

UdpServer::~UdpServer() {
    std::cout << "UDP server stopped" << std::endl;
    _timer.join();
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
            sendAll("new:" + std::to_string(_clients[_sender_endpoint].getId()) + "," + getPositionString(_clients[_sender_endpoint].getPosition()),
                    false);
            // send all players to new player
            for (auto &client : _clients) {
                if (client.first != _sender_endpoint) {
                    sendResponse(_sender_endpoint,
                                 "new:" + std::to_string(client.second.getId()) + "," + getPositionString(client.second.getPosition()));
                }
            }
        }
        auto &player = _clients[_sender_endpoint];
        std::cout << "Player: " << player.getId() << std::endl;
        if (msg == "ping") {
            sendResponse(_sender_endpoint, "pong");
            player.setAlive(true);
        }
        if (msg == "disconnect") {
            removeClient(_sender_endpoint);
        }
        if (isMatch(msg, "^move:(left|right|up|down)," NB_R "," NB_R "$")) {
            auto match = getMatches(msg, "^move:(left|right|up|down)," NB_R "," NB_R "$");
            auto direction = match[1];
            auto x = std::stof(match[2]);
            auto y = std::stof(match[3]);
            auto pos = Vector2{x, y};
            auto valid = player.move(direction, pos);
            sendAll("move:" + std::to_string(player.getId()) + "," + getPositionString(player.getPosition()), !valid);
        }
    }
}

void UdpServer::sendResponse(const udp::endpoint &endpoint, const std::string &msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _socket.send_to(asio::buffer(msg), endpoint);
}

void UdpServer::receiveRequest() {
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
