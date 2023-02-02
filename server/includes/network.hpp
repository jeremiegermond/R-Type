#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <asio.hpp>
#include <chrono>
#include <cmath>
#include <cstring>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <queue>
#include <string>
#include <thread>
#include <utility>

using asio::ip::udp;
using namespace std::chrono_literals;

std::vector<std::string> split(const std::string &s, char delim);
std::string floatToString(float number, int precision = 1);

class Player {
  private:
    std::array<float, 2> _position;
    unsigned int _id;
    int _hp;

  public:
    Player(unsigned int id) : _id(id), _position({0, 0}), _hp(100) {}
    Player(int id, std::array<float, 2> position, int hp) : Player(id) {
        _position = position;
        _hp = hp;
    }
    std::array<float, 2> getPos() { return _position; }
    int getHp() const { return _hp; }
    int getId() const { return _id; }
    void setPos(std::array<float, 2> pos) { _position = pos; }
    void setHp(int hp) { _hp = hp; }
};

class Session {
  private:
    Player *_player = nullptr;
    std::vector<Player *> _players;

  public:
    Session() {}
    explicit Session(int id) { _player = new Player(id, {0, 0}, 100); }
    explicit Session(Player *player) { _player = player; }
    int getPlayerId() { return _player->getId(); }
    Player *getPlayer() { return _player; }
    Player *getPlayer(int id) {
        for (auto &player : _players) {
            if (player->getId() == id)
                return player;
        }
        return nullptr;
    }
    std::vector<Player *> getPlayers() { return _players; }
    void addPlayer(Player *player) { _players.push_back(player); }
    void removePlayer(int id) {
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->getId() == id) {
                _players.erase(_players.begin() + i);
                break;
            }
        }
    }
    void updatePlayerPos(int id, std::array<float, 2> pos) {
        for (auto &player : _players) {
            if (player->getId() == id) {
                player->setPos(pos);
                break;
            }
        }
    }
};

class id_generator {
  private:
    std::vector<unsigned int> ids;

  public:
    id_generator() = default;
    unsigned int get_unique_id() {
        unsigned int tmp = rand();
        while (std::find(ids.begin(), ids.end(), tmp) != ids.end())
            tmp = rand();
        return tmp;
    }
};

class UdpRequest {
  private:
    std::string _data;
    std::string _response;
    unsigned int _id;
    int _state = 0; // -1: not processed,  0: pending, 1: _response received, 2: timeout
  public:
    UdpRequest(std::string data, unsigned int id) : _data(std::move(data)), _id(id) {}
    std::string get_data() { return _data; }
    [[nodiscard]] unsigned int get_id() const { return _id; }
    [[nodiscard]] int get_state() const { return _state; }
    std::string get_response() { return _response; }
    bool operator==(int num) const {
        if (_id == num) {
            return true;
        }
        return false;
    }
    void set_response(std::string data) {
        _response = std::move(data);
        _state = 1;
    }
    void setState(int new_state) { _state = new_state; }
};

class Client {
  private:
    std::array<float, 2> _serverPos;
    int _id;

  public:
    explicit Client(int id) : _serverPos({0, 0}), _id(id) {}
    int getId() const { return _id; }
    bool move(const std::string &direction, std::array<float, 2> &clientPos) {
        if (direction == "right")
            _serverPos[0] += .1;
        if (direction == "left")
            _serverPos[0] -= .1;
        if (direction == "up")
            _serverPos[1] += .1;
        if (direction == "down")
            _serverPos[1] -= .1f;
        std::cout << "_serverPos: " + floatToString(_serverPos[0]) + ":" + floatToString(_serverPos[1]) << std::endl;
        std::cout << "clientPos: " + floatToString(clientPos[0]) + ":" + floatToString(clientPos[1]) << std::endl;
        if (floatToString(_serverPos[0]) != floatToString(clientPos[0]) || floatToString(_serverPos[1]) != floatToString(clientPos[1]))
            return false;
        return true;
    }
    std::array<float, 2> getServerPos() { return _serverPos; }
    bool processMovement(const std::string &data, std::array<float, 2> &clientPos) {
        if (data == "right" || data == "left" || data == "up" || data == "down") {
            if (!move(data, clientPos))
                return false;
            return true;
        }
        return false;
    }
};

class UdpServer {
  private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    std::array<char, 1024> recv_buf_{};
    std::map<asio::ip::udp::endpoint, Client *> clients;

    void do_receive();

    void do_send(std::size_t length);

  public:
    UdpServer(asio::io_context &io_context, short port) : socket_(io_context, udp::endpoint(udp::v4(), port)) { do_receive(); }
};
