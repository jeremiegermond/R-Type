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
    std::array<float, 2> pos;
    int id;
    int hp;

  public:
    Player(unsigned int id) {
        this->id = id;
        pos.fill(0);
        hp = 100;
    }
    Player(int id, std::array<float, 2> pos, int hp) {
        this->id = id;
        this->pos = pos;
        this->hp = hp;
    }
    std::array<float, 2> get_pos() { return pos; }
    int get_hp() { return hp; }
    int get_id() { return id; }
    void set_pos(std::array<float, 2> pos) { this->pos = pos; }
    void set_hp(int hp) { this->hp = hp; }
};

class Session {
  private:
    Player *_player = nullptr;
    std::vector<Player *> _players;

  public:
    Session() {}
    Session(unsigned int id) { _player = new Player(id, {0, 0}, 100); }
    Session(Player *player) { this->_player = player; }
    unsigned int get_player_id() { return _player->get_id(); }
    Player *get_player() { return _player; }
    Player *get_player(unsigned int id) {
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->get_id() == id)
                return _players[i];
        }
        return nullptr;
    }
    std::vector<Player *> get_players() { return _players; }
    void add_player(Player *player) { _players.push_back(player); }
    void remove_player(int id) {
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->get_id() == id) {
                _players.erase(_players.begin() + i);
                break;
            }
        }
    }
    void update_player_pos(int id, std::array<float, 2> pos) {
        for (int i = 0; i < _players.size(); i++) {
            if (_players[i]->get_id() == id) {
                _players[i]->set_pos(pos);
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
    std::string response;
    unsigned int id;
    int state = 0; // -1: not processed,  0: pending, 1: response received, 2: timeout
  public:
    UdpRequest(std::string data, unsigned int id) : _data(std::move(data)), id(id) {}
    std::string get_data() { return _data; }
    [[nodiscard]] unsigned int get_id() const { return id; }
    [[nodiscard]] int get_state() const { return state; }
    std::string get_response() { return response; }
    bool operator==(int num) const {
        if (id == num) {
            return true;
        }
        return false;
    }
    void set_response(std::string data) {
        response = std::move(data);
        state = 1;
    }
    void set_state(int new_state) { state = new_state; }
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

    void dispatcher(std::string data);

    bool disconnected();

    void loopHeartbeat();

    bool sendConnect();

    bool sendHeartbeat(bool first_connection = false);
    bool alive();
    std::vector<Player *> get_players();
    ~UdpClient();
};
