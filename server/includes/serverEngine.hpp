#pragma once
#include "EngineUdpServer.hpp"
#include "network.hpp"
#include "room.hpp"
#include "uiElements.hpp"

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
        // SetTraceLogCallback(customLog);
        SetTargetFPS(60);
        font = LoadFontEx("./assets/fonts/Poppins-Medium.ttf", 128, 0, 250);
        init_dashboard();
        _server = std::thread([this]() {
            EngineUdpServer _server(4242, _logs);
            //_server.start(_rooms);
            while (running) {
                std::string cmd = _server.getRequest();
                if (cmd == "getRooms") {
                    std::string buffer = "rooms:";
                    for (auto room : _rooms)
                        buffer += std::to_string(room->get_port()) + ":" + std::to_string(room->get_player_count()) + ',';
                    buffer.pop_back();
                    _server.sendResponse(buffer); // send list of rooms ports and infos
                } else if (cmd.substr(0, cmd.find_first_of(":")) == "createRoom") {
                    int port = std::stoi(cmd.substr(cmd.find_first_of(":") + 1));
                    createRoom(port);
                    _server.sendResponse(std::to_string(port)); // send port of the new room
                } else if (cmd == "ping")
                    _server.sendResponse("pong");
                else {
                    _server.sendResponse("error");
                }
            }
        });
        _server.detach();
    };
    ~server_engine() { running = false; };
    void init_dashboard() {
        _interface.add(new textButton({25, 200, 175, 50}, WHITE, "Create Room", BLACK, [this](){
            static uiElement *elem = nullptr;
            if (elem)
                return;
            elem = new textbox({ 215, 225, 150, 22 }, DARKGRAY, WHITE, [this](std::string tmp){
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
        _interface.focus(_interface.add(new textbox({ 50, 425, 700, 22 }, DARKGRAY, WHITE, [this](std::string tmp){
            handle_input(tmp);
        }, "> ")));
        _interface.add(new text("/*** RType Server ***\\", {75, 0, 30, 1}, WHITE));
        _interface.add(new text("Available Rooms :", {550, 35, 25, 1}, WHITE));


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
                update_log();
                _interface.update();
                _interface.draw();
            } else {
                if (!_current_room->isRunning())
                    _current_room = nullptr;
                _current_room->displayLog();
                _current_room->displayPlayer();
                _current_room->displayEnemies();
                _current_room->displayBullets();
                _current_room->displayOverlay();
                _overlay.update();
                _overlay.draw();
            }
            EndDrawing();
            for (auto room: _rooms)
                if (!room->isRunning())
                    deleteRoom(room->get_port());
        }
    };
    void createRoom(int port) {
        for (auto room: _rooms)
            if (*room == port) {// create exception
                return;
            }
        _rooms.push_back(new Room(port));
        _interface.add(new textButton({550, 50 + float(_rooms.size()) * 25, 200, 25}, Color{38, 38, 38, 255}, std::to_string(port), WHITE, [&](){
            textButton *button = dynamic_cast<textButton *>(_interface.getFocused());
            for (auto room: _rooms)
                if (*room == std::stoi(button->getText())) {
                    _current_room = room;
                    break;
                }
        }))->addClass(std::to_string(port));
        log(LOG_INFO, "Created room " + std::to_string(port));
    }
    void deleteRoom(int port) {
        for (auto room: _rooms)
            if (*room == port) {
                delete room;
                _rooms.erase(std::find(_rooms.begin(), _rooms.end(), room));
                _interface.remove(_interface.getClass(std::to_string(port))[0]);
                log(LOG_INFO, "Deleted room " + std::to_string(port));
                return;
            }
        log(LOG_ERROR, "Room " + std::to_string(port) + " not found");
    }
    void listRooms() {
        for (auto room: _rooms)
            std::cout << room->get_port() << std::endl;
    }
    void handle_input(std::string input) {
        std::string command = input.substr(0, input.find(" "));
        std::string ret = "";
        if (input.substr(0, 4) == "exit")
            running = false;
        else if (input.substr(0, 10) == "createRoom") {
            createRoom(std::stoi(input.substr(11)));
        } else if (input.substr(0, 8) == "getRooms") {
            for (auto room : _rooms)
                if(room->isRunning())
                    ret += std::to_string(room->get_port()) + ":" + std::to_string(room->get_player_count()) + ',';
            if (ret != "")
                ret.pop_back();
        } else if (input.substr(0, 10) == "deleteRoom") {
            deleteRoom(std::stoi(input.substr(11)));
        } else if (input.substr(0, 4) == "help") {
            ret = "Available commands : \n";
            ret += "exit : exit the server\n";
            ret += "createRoom <port> : create a room on the specified port\n";
            ret += "getRooms : get the list of rooms\n";
            ret += "deleteRoom <port> : delete the room on the specified port\n";
            ret += "clear : clear the log\n";
        } else if (input.substr(0, 5) == "clear") {
            for (auto elem : _interface.getClass("log"))
                _interface.remove(elem);
        } else
            ret = "Unknown command";
        if (ret != "")
            log(LOG_WARNING, ret);
    }
        
    void create_log(int type, const std::string &msg) {
        static float pos = 0;
        if (_interface.getClass("log").size() == 0)
            pos = 0;
        _interface.add(new text(msg, { 10, 40 + pos, 20, 1 }, type != LOG_INFO ? RED : GRAY))->addClass("log");
        if (pos >= 130) {
            uiElement *elem = _interface.getClass("log").front();
            _interface.remove(elem);
            for (auto elem : this->_interface.getClass("log"))
                elem->setPos({elem->getPos().x, elem->getPos().y - 17});
        } else
            pos += 17;
    };

    void log(int type, const std::string &msg) { _logs.push_back(std::make_pair(type, msg)); };

    void update_log() {
        for (auto log : _logs)
            create_log(log.first, log.second);
        _logs.clear();
    };

  private:
    bool running = true;
    asio::io_context _io_context;
    interface _interface;
    interface _overlay;
    std::vector<Room *> _rooms;
    std::thread _server;
    Room *_current_room = nullptr;
    std::vector<std::pair<int, std::string>> _logs;
};
