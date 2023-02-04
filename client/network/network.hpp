#pragma once

#include "deps.hpp"
#include "utils.hpp"

#include <arpa/inet.h>
#include <array>
#include <cstring>
#include <functional>
#include <future>
#include <iomanip>
#include <netinet/in.h>
#include <poll.h>
#include <queue>
#include <random>
#include <string>
#include <utility>

using namespace std::chrono_literals;

class Player {
  private:
    Vector2 _position;
    int _id;
    int _hp;

  public:
    explicit Player(int id) : _id(id), _position({0, 0}), _hp(100) {}
    Player(int id, Vector2 pos, int hp) : Player(id) {
        _position = pos;
        _hp = hp;
    }
    Vector2 getPos() { return _position; }
    [[nodiscard]] int getHp() const { return _hp; }
    [[nodiscard]] int getId() const { return _id; }
    void setPos(Vector2 pos) { this->_position = pos; }
    void setHp(int hp) { this->_hp = hp; }
};

class Session {
  private:
    Player *_player = nullptr;
    std::vector<Player *> _players;

  public:
    Session() = default;
    explicit Session(int id) { _player = new Player(id, {0, 0}, 100); }
    explicit Session(Player *player) { _player = player; }
    unsigned int getPlayerId() { return _player->getId(); }
    Player *getPlayer() { return _player; }
    Player *getPlayer(unsigned int id) {
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
    void updatePlayerPos(int id, Vector2 pos) {
        for (auto &player : _players) {
            if (player->getId() == id) {
                player->setPos(pos);
                break;
            }
        }
    }
};

class idGenerator {
  private:
    std::vector<unsigned int> ids;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

  public:
    // Clang-Tidy: Random number generator seeded with a default argument will generate a predictable sequence of values
    idGenerator() {
        // Warning: this is not a good way to seed the generator
        std::random_device rd;
        gen = std::mt19937(rd());
        // This is the good way to seed the generator:
        // std::random_device rd;
        // std::mt19937 gen(rd());

        dis = std::uniform_int_distribution<>(0, 1000000);
        for (int i = 0; i < 1000000; i++) {
            ids.push_back(dis(gen));
        }
    }
    // to fix the warning, we have to change the function to this
    // _generator() {
    //     // seed
    //     gen = std::mt19937(rd());
    //     dis = std::uniform_int_distribution<>(0, 1000000);
    int getUniqueId() {
        int id = ids.back();
        ids.pop_back();
        return id;
    }
};

class UdpRequest {
  private:
    std::string _request;
    std::string _response;
    int _state = 0; // -1: not processed,  0: pending, 1: _response received, 2: timeout
  public:
    UdpRequest() = default;
    explicit UdpRequest(std::string data) : _request(std::move(data)) {}
    virtual ~UdpRequest() = default;
    std::string getRequest() { return _request; }
    [[nodiscard]] int getState() const { return _state; }
    std::string getResponse() { return _response; }
    void setResponse(std::string data) {
        _response = std::move(data);
        setState(1);
    }
    void setState(int newState) { _state = newState; }
};

typedef std::unordered_map<int, UdpRequest *> requestMap;
class UdpClient {
  private:
    struct pollfd *pfd;
    struct sockaddr_in sockaddrIn;
    socklen_t socklen;
    std::thread heartbeat;
    std::thread listener;
    std::string server_ip;
    unsigned short server_port;
    std::atomic<int> _isAlive = -1; // -1: dead, 0: unknown, 1: alive
    std::atomic<bool> shutdown = false;
    requestMap requests; // string to send to the server
    // requests to delete
    // std::vector<int> requestsToDelete;
    idGenerator _generator;
    std::atomic<bool> connected = false;
    Session *session;

  public:
    UdpClient();
    bool connect(const std::string &ip, unsigned short port);

    void sendRequest(const std::string &data);

    void send(std::string data, bool showDebug = false);

    requestMap get_requests();

    void popRequest(int id);

    //  add to requests to delete
    // void deleteRequest(int id);

    // void show_requests();

    void loopListener();

    void receive(bool showDebug = false);

    void handleRequest(const std::string &data);

    bool disconnected();

    void loopHeartbeat();

    bool sendConnect();

    bool sendHeartbeat(bool first_connection = false);
    bool alive();
    std::vector<Player *> getPlayers();
    ~UdpClient();
};
