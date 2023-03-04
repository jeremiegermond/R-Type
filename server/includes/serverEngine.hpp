#pragma once
#include "uiElements.hpp"
#include "network.hpp"
#include "MainServer.hpp"
#include "room.hpp"

/*
    TODO :
        - simplify server & rooms
        - display room infos (nb of player, ...) on dashboard
        - close room from dashboard
        - display log on dashboard
        - catch errors
        - moove classes to files
*/

class server_engine {
    public:
        server_engine() {
            InitWindow(800, 450, "R-Type Server");
            //SetTraceLogCallback(customLog);
            SetTargetFPS(60);
            font = LoadFontEx("./assets/fonts/Poppins-Medium.ttf", 128, 0, 250);
            init_dashboard();
            _server = std::thread([this](){
                MainServer _server(4242);
                //_server.start(_rooms);
                while (running) {
                    std::string cmd = _server.getRequest();
                    if (cmd == "getRooms") {
                        std::string buffer;
                        for (auto room : _rooms) {
                            buffer += std::to_string(room->get_port()) + ":" + std::to_string(room->get_player_count()) + ',';
                        }
                        buffer.pop_back();
                        _server.sendResponse(buffer);  // send list of rooms ports and infos
                    } else if (cmd.substr(0, cmd.find_first_of(":")) == "createRoom") {
                        int port = std::stoi(cmd.substr(cmd.find_first_of(":") + 1));
                        Room *room = new Room(port);
                        _server.sendResponse(std::to_string(port));  // send port of the new room
                    }
                }
            });
            _server.detach();
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
        std::thread _server;
        Room *_current_room = nullptr;
};