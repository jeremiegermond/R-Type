#pragma once

#include "bullet.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "uiElements.hpp"
#include <set>

using namespace asio::ip;
using namespace std::chrono;

namespace std {
    template <>
    struct hash<udp::endpoint> {
        std::size_t operator()(const udp::endpoint &endpoint) const noexcept {
            std::size_t h1 = std::hash<std::string>()(endpoint.address().to_string());
            std::size_t h2 = std::hash<unsigned short>()(endpoint.port());
            return h1 ^ (h2 << 1);
        }
    };
}

/**
 * @brief UDP server that handle multiple clients
 * 
 */
class UdpServer {
  private:
    asio::io_context _io_context;
    udp::socket _socket;
    std::unordered_map<udp::endpoint, Player> _clients;
    std::set<int> _ids;
    udp::endpoint _sender_endpoint;
    std::array<char, 1024> _buffer{};
    std::thread _timer;
    std::thread _simulation;
    // mutex for socket endpoint
    std::mutex _socketMutex;
    // bullets
    std::vector<Bullet> _bullets;
    // enemies
    std::vector<Enemy> _enemies;
    std::set<int> _enemyIds;
    // timer for enemy spawn
    std::chrono::time_point<std::chrono::steady_clock> _enemySpawnTimer;
    // frame time
    std::chrono::time_point<std::chrono::steady_clock> _lastFrame;
    // stop variable
    std::atomic<bool> _stopServer;
    int _port;
    std::vector<std::pair<int, std::string>> _logs;

    interface _overlay;

  public:
    /**
     * @brief Construct a new Udp Server object
     * 
     * @param port 
     */
    UdpServer(int port) : _socket(_io_context, udp::endpoint(udp::v4(), port)), _stopServer(false) {
        _port = port;
        for (int i = 1; i <= 4; i++)
            _ids.insert(i);
        for (int i = 1; i <= 10; i++)
            _enemyIds.insert(i);
        _overlay.add(new text("Room : " + std::to_string(_port), {0, 30, 25, 1}, WHITE));
        _overlay.add(new text("Players : ", {0, 50, 25, 1}, WHITE));
        _overlay.add(new text(std::to_string(_clients.size()), {95, 50, 25, 1}, WHITE))->setId("players_nbr");
        _overlay.add(new text("/ 4", {110, 50, 25, 1}, WHITE));
    }
    /**
     * @brief Destroy the Udp Server object
     * 
     */
    ~UdpServer();

    /**
     * @brief start the server
     * 
     */
    void start();

    /**
     * @brief stop the server
     * 
     */
    void stop(){ _io_context.stop(); _stopServer = true; }

    /**
     * @brief handle request from client
     * 
     * @param ec 
     * @param bytes_recvd 
     */
    void handleRequest(std::error_code ec, std::size_t bytes_recvd);

    /**
     * @brief send response to client
     * 
     * @param endpoint 
     * @param msg 
     */
    void sendResponse(const udp::endpoint &endpoint, const std::string &msg);

    /**
     * @brief receive request from client
     * 
     */
    void receiveRequest();

    /**
     * @brief check if client still alive
     * 
     */
    void checkAlive();

    /**
     * @brief send message to all clients
     * 
     * @param msg 
     * @param includeSender 
     */
    void sendAll(const std::string &msg, bool includeSender = true);

    /**
     * @brief remove client from server
     * 
     * @param endpoint 
     * @param erase 
     */
    void removeClient(const udp::endpoint &endpoint, bool erase = true);

    /**
     * @brief simulate game
     * 
     */
    void simulate();

    /**
     * @brief log the message -> msg
     * 
     * @param type 
     * @param msg 
     */
    void log(int type, const std::string &msg) { _logs.emplace_back(type, msg); };
    /**
     * @brief check if server is running
     * 
     * @return int 
     */
    int is_running() { return !_stopServer; }
    /**
     * @brief Create a log object
     * 
     * @param type 
     * @param msg 
     */
    void create_log(int type, const std::string &msg) {
        static float pos = 0;
        _overlay.add(new text(msg, { 0, 100 + pos, 18, 1 }, type != LOG_INFO ? RED : GRAY))->addClass("log");
        if (pos >= 320) {
            uiElement *elem = _overlay.getClass("log").front();
            _overlay.remove(elem);
            for (auto elem : this->_overlay.getClass("log"))
                elem->setPos({elem->getPos().x, elem->getPos().y - 14});
        } else
            pos += 14;
    };
    /**
     * @brief update the logs
     * 
     */
    void update_log() {
        for (auto log : _logs)
            create_log(log.first, log.second);
        _logs.clear();
    };

    /**
     * @brief Get the Clients object
     * 
     * @return std::unordered_map<udp::endpoint, Player> 
     */
    std::unordered_map<udp::endpoint, Player> getClients() { return _clients; }

    /**
     * @brief Get the Enemies object
     * 
     * @return std::vector<Enemy> 
     */
    std::vector<Enemy> getEnemies() { return _enemies; }

    /**
     * @brief Get the Bullets object
     * 
     * @return std::vector<Bullet> 
     */
    std::vector<Bullet> getBullets() { return _bullets; }

    /**
     * @brief Get the Overlay object
     * 
     * @return interface* 
     */
    interface *getOverlay() { return &_overlay; }
};
