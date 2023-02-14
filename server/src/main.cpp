#include "network.hpp"
#include "utils.hpp"
#include <queue>

std::unordered_map<int, Color> _log_colors = {
    {LOG_INFO, WHITE},
    {LOG_ERROR, RED},
    {LOG_WARNING, YELLOW},
    {LOG_DEBUG, BLUE},
};    

class Room {
    public:
        Room(int port): server(port) {

        font = LoadFont("./assets/fonts/console.otf");// moove
            _port = port;
            _thread = std::thread([this](){
                running = true;
                server.start();
                running = false;
            });
        };
        ~Room() {
            if (running) {
                _thread.join();
            }
        };
        bool isRunning() {
            return running;
        };

        void displayLog() {
            float y = 0;
            auto logs = server.getLog();
            for (auto &log : *logs) {
                DrawTextEx(font, log.second.c_str(), Vector2{10, y}, 20, 1, _log_colors[log.first]);
                if (y > 380) {
                    logs->erase(logs->begin());
                } else
                    y += 20;
            }
            logs->shrink_to_fit();
        };

        void displayPlayer() {
            for (auto &client : server.getClients()) {
                auto player = client.second;
                auto position = player.getPosition();
                DrawText("p", (position.x + 10) * 40, (-position.y + 5) * 40, 20, GREEN);
            }
        };

        void displayEnemies() {
            for (auto &enemy : server.getEnemies()) {
                auto position = enemy.getPosition();
                DrawText("x", (position.x + 10) * 40, (-position.y + 5) * 40, 20, RED);
            }
        };

        void displayBullets() {
            for (auto &bullet : server.getBullets()) {
                auto position = bullet.getPosition();
                DrawText("o", (position.x + 10) * 40, (-position.y + 5) * 40, 20, YELLOW);
            }
        };
    private:
        int _port;
        bool running;
        std::thread _thread;
        UdpServer server;
            Font font;//to moove
};

class RTypeServer{
    public:
        RTypeServer(){
            font = LoadFont("./assets/fonts/console.otf");
        };
        ~RTypeServer(){
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
        void display_none() {
            displayed_room = nullptr;
        };
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
            for (auto &log : _logs) {// duplicate code
                DrawTextEx(font, log.second.c_str(), Vector2{200, y}, 20, 1, _log_colors[log.first]);
                if (y > 380)
                    _logs.pop_back();
                else
                    y += 20;
            }
        };

        void log(int type, const std::string &msg) {
            _logs.emplace_back(type, msg);
        };
    private:
        asio::io_context _io_context;
        UdpServer *_udpServer;
        std::map<int, Room*> _rooms;
        Room *displayed_room = nullptr;
        std::vector<std::pair<int, std::string>> _logs;
        Font font;
    
};
void customLog(int logType, const char *text, va_list args) {
    
}

void handle_input(RTypeServer &_server, std::string input) { // moove to UdpServer class
    std::string command = input.substr(0, input.find(" "));
    if (input.substr(0, 4) == "exit")
        return;
    else if (input.substr(0, 10) == "createRoom") {
        _server.createRoom(std::stoi(input.substr(11)));
        _server.display(std::stoi(input.substr(11)));
    } else if (input.substr(0, 4) == "list")
        _server.listRoom();
    else if (input.substr(0, 7) == "display") {
        _server.display_none();
        if (input.size() > 8)
            _server.display(std::stoi(input.substr(8)));
    }
    else
        std::cout << "Unknown command" << std::endl;
    input = "";
}

int main() {
    asio::io_context _io_context;
    std::queue<std::string> _input;
        InitWindow(800, 450, "R-Type Server");
        SetTraceLogCallback(customLog);
        SetTargetFPS(60);
    RTypeServer _server;
    std::thread t([&](){
        std::string tmp;
        while (true) {
            std::getline(std::cin, tmp);
            _input.push(tmp);
        }
    });
    while (!WindowShouldClose()) {
        if (_input.size() > 0) {
            handle_input(_server, _input.front());
            _input.pop();
        }
        //_server.display_infos();
        _server.display_current_room();
    }


    
    return 0;
}
