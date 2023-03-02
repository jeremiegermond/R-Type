#pragma once
#include "uiElements.hpp"
#include "network.hpp"
/*#include "TcpServer.hpp"*/

/*
    TODO :
        - simplify server & rooms
        - display room infos (nb of player, ...) on dashboard
        - close room from dashboard
        - display log on dashboard
        - catch errors
*/

class Room {
    public:
        Room(int port): server(port) {

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

        int get_port() {
            return _port;
        }

        void displayLog() {
            float y = 0;
            auto logs = server.getLog();
            for (auto &log : *logs) {
                //DrawTextEx(font, log.second.c_str(), Vector2{10, y}, 20, 1, _log_colors[log.first]);
                if (y > 380) {
                    logs->erase(logs->begin());
                } else
                    y += 20;
            }
            logs->shrink_to_fit();
        };

        void displayOverlay() {
            interface *overlay = server.getOverlay();
            overlay->update();
            overlay->draw();
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

        bool operator==(int port) {
            return _port == port;
        }
    private:
        int _port;
        bool running;
        std::thread _thread;
        UdpServer server;
           // Font font;//to moove
};

class server_engine {
    public:
        server_engine() {
            exit(0);
            InitWindow(800, 450, "R-Type Server");
            //SetTraceLogCallback(customLog);
            SetTargetFPS(60);
            font = LoadFontEx("./assets/fonts/Poppins-Medium.ttf", 128, 0, 250);
            init_dashboard();
        };
        ~server_engine() {
            running = false;
        };
        void init_dashboard() {
            _interface.add(new textButton({25, 200, 175, 50}, WHITE, "Create Room", BLACK, [this](){
                static uiElement *elem = nullptr;
                std::cout << elem << std::endl;
                if (elem)
                    return;
                std::cout << "Create Room" << std::endl;
                elem = new textbox({ 215, 225, 150, 22 }, DARKGRAY, WHITE, [this](std::string tmp){
                    std::cout << tmp << std::endl;
                    _interface.remove(elem);
                    elem = nullptr;
                    createRoom(std::stoi(tmp));
                }, "Port : ");
                elem->onUnfocus([this](){
                    _interface.remove(elem);
                    elem = nullptr;
                });
                _interface.add(elem);
                _interface.focus(elem);
            }));
            _interface.add(new textButton({25, 275, 175, 50}, RED, "Exit", WHITE, [this](){
                running = false;
            }));
            _interface.add(new textbox({ 50, 425, 700, 22 }, DARKGRAY, WHITE, [this](std::string tmp){
                std::cout << tmp << std::endl;
            }, "> "));
            _interface.add(new text("/*** RType Server ***\\", {20, 0, 40, 2}, WHITE));
            _interface.add(new text("Available Rooms :", {550, 35, 30, 2}, WHITE));


            _overlay.add(new textButton({0, 0, 30, 30}, BLACK, "<-", WHITE, [this](){
                _current_room = nullptr;
            }));
            createRoom(12345);
        };
        void run() {
            while (!WindowShouldClose() && running) {
                BeginDrawing();
                ClearBackground(Color{38, 38, 38, 255});
                if (_current_room == nullptr) {
                    _interface.update();
                    _interface.draw();
                } else {
                    _current_room->displayLog();
                    _current_room->displayPlayer();
                    _current_room->displayEnemies();
                    _current_room->displayBullets();
                    _current_room->displayOverlay();
                    _overlay.update();
                    _overlay.draw();
                }
                EndDrawing();
            }
        };
        void createRoom(int port) {
            for (auto room: _rooms)
                if (*room == port) {
                    std::cout << "Room " << port << " already exist" << std::endl;
                    return;
                }
            _rooms.push_back(new Room(port));
            _interface.add(new textButton({550, 50 + float(_rooms.size()) * 25, 200, 25}, Color{38, 38, 38, 255}, std::to_string(port), WHITE, [&](){
                textButton *button = dynamic_cast<textButton *>(_interface.getFocused());
                std::cout << "Switch to room " << button->getText() << std::endl;
                for (auto room: _rooms)
                    if (*room == std::stoi(button->getText())) {
                        _current_room = room;
                        break;
                    }
            }));
            std::cout << "Created room " << port << std::endl;
        }
        void listRooms() {
            for (auto room: _rooms)
                std::cout << room->get_port() << std::endl;
        }
    private:
        bool running = true;
        asio::io_context _io_context;
        interface _interface;
        interface _overlay;
        std::vector<Room *> _rooms;
        //TcpServer _server;
        Room *_current_room = nullptr;
};