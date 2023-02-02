#pragma once

#include "utils.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <chrono>
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
using namespace std::chrono_literals;

class Player {
  private:
    std::array<float, 2> _position;
    int _id;
    int _hp;

  public:
    Player(unsigned int id) {
        this->_id = id;
        _position.fill(0);
        _hp = 100;
    }
    Player(int id, std::array<float, 2> pos, int hp) {
        this->_id = id;
        this->_position = pos;
        this->_hp = hp;
    }
    std::array<float, 2> getPos() { return _position; }
    int getHp() { return _hp; }
    int getId() { return _id; }
    void setPos(std::array<float, 2> pos) { this->_position = pos; }
    void setHp(int hp) { this->_hp = hp; }
};

class Session {
  private:
    Player *_player = nullptr;
    std::vector<Player *> _players;

  public:
    Session() {}
    Session(unsigned int id) { _player = new Player(id, {0, 0}, 100); }
    Session(Player *player) { this->_player = player; }
    unsigned int getPlayerId() { return _player->getId(); }
    Player *getPlayer() { return _player; }
    Player *getPlayer(unsigned int id) {
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->getId() == id)
                return _players[i];
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
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->getId() == id) {
                _players[i]->setPos(pos);
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

class UdpClient {
  private:
    struct pollfd *pfd;
    struct sockaddr_in servaddr;
    socklen_t len;
    std::thread heartbeat;
    std::thread listener;
    std::string server_ip;
    unsigned short server_port;
    std::atomic<int> is_alive = -1; // -1: dead, 0: unknown, 1: alive
    std::atomic<bool> shutdown = false;
    std::vector<UdpRequest *> requests; // string to send to the server
    id_generator rid;
    std::atomic<bool> connected = false;
    Session *session;

  public:
    UdpClient();
    bool connect(const std::string &ip, unsigned short port);

    void request(std::string data);

    void send(std::string data, bool showDebug = false);

    std::vector<UdpRequest *> get_requests();

    void pop_request(unsigned int id);

    void show_requests();

    void loopListener();

    void receive(bool showDebug = false);

    void dispatcher(const std::string &data);

    bool disconnected();

    void loopHeartbeat();

    bool sendConnect();

    bool sendHeartbeat(bool first_connection = false);
    bool alive();
    std::vector<Player *> get_players();
    ~UdpClient();
};
