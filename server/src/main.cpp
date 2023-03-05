#include "network.hpp"
#include "serverEngine.hpp"
#include "utils.hpp"
#include <queue>

std::unordered_map<int, Color> _log_colors = {
    {LOG_INFO, WHITE},
    {LOG_ERROR, RED},
    {LOG_WARNING, YELLOW},
    {LOG_DEBUG, BLUE},
};

class RTypeServer {
  public:
    RTypeServer(){};
    ~RTypeServer() {
        for (auto &room : _rooms) {
            delete room.second;
        }
    };
    void createRoom(int port) {
        if (_rooms.find(port) != _rooms.end()) {
            if (!_rooms[port]->isRunning())
                delete _rooms[port];
            else {
                std::cout << "Room already exist" << std::endl;
                return;
            }
        }
        _rooms[port] = new Room(port);
    };
    void listRoom() {
        for (auto &room : _rooms) {
            std::cout << room.first << std::endl;
        }
    };
    void display(int port) {
        if (_rooms.find(port) != _rooms.end()) {
            if (_rooms[port]->isRunning())
                displayed_room = _rooms[port];
            else
                std::cout << "Room is not running" << std::endl;
        } else {
            std::cout << "Room does not exist" << std::endl;
        }
    };
    void display_none() { displayed_room = nullptr; };
    void display_current_room() {
        BeginDrawing();
        ClearBackground(BLACK);
        if (displayed_room != nullptr) {
            displayed_room->displayPlayer();
            displayed_room->displayEnemies();
            displayed_room->displayBullets();
            displayed_room->displayLog();
        }
        EndDrawing();
    };

    void display_infos() {
        float y = 20;
        for (auto &log : _logs) { // duplicate code
            // DrawTextEx(font, log.second.c_str(), Vector2{200, y}, 20, 1, _log_colors[log.first]);
            if (y > 380)
                _logs.pop_back();
            else
                y += 20;
        }
    };

    void log(int type, const std::string &msg) { _logs.emplace_back(type, msg); };

  private:
    asio::io_context _io_context;
    UdpServer *_udpServer;
    std::map<int, Room *> _rooms;
    Room *displayed_room = nullptr;
    std::vector<std::pair<int, std::string>> _logs;
};

int main() {
    server_engine server;
    server.run();
    return 0;
}
