#pragma once

#include "utils.hpp"
#include "room.hpp"

/*
    TODO:
        - merge this class and UdpServer
*/

using asio::ip::tcp;

class MainServer {
    public:
        MainServer(int port) : _socket(_io_context, udp::endpoint(udp::v4(), port)) {
            _port = port;
        }

        ~MainServer() {
            _stopServer = true;
            _io_context.stop();
        }

        // void start(std::vector<Room *> &rooms) {
        //     log(LOG_INFO, "UDP server started on port " + std::to_string(_port));
        //     _rooms = &rooms;
        //     receiveRequest();
        //     _io_context.run();
        // }

        int receiveRequest() {
            if (_stopServer)
                return 0;
            return _socket.receive_from(asio::buffer(_buffer), _sender_endpoint);
        }

        void sendResponse(const std::string &msg) {
            std::lock_guard<std::mutex> lock(_socketMutex);
            _socket.send_to(asio::buffer(msg), _sender_endpoint);
        }

        void createRoom(int port) { // duplicate
            for (auto room: *_rooms)
                if (*room == port) {
                    std::cout << "Room " << port << " already exist" << std::endl;
                    return;
                }
            _rooms->push_back(new Room(port));
            // _interface.add(new textButton({550, 50 + float(_rooms.size()) * 25, 200, 25}, Color{38, 38, 38, 255}, std::to_string(port), WHITE, [&](){
            //     textButton *button = dynamic_cast<textButton *>(_interface.getFocused());
            //     std::cout << "Switch to room " << button->getText() << std::endl;
            //     for (auto room: _rooms)
            //         if (*room == std::stoi(button->getText())) {
            //             _current_room = room;
            //             break;
            //         }
            // }));
            std::cout << "Created room " << port << std::endl;
        }

        std::string getRequest() {
            int bytes_recvd = receiveRequest();
            if (bytes_recvd <= 0)
                return "";
            std::string msg(_buffer.begin(), _buffer.begin() + bytes_recvd);
            log(LOG_INFO, "From: " + _sender_endpoint.address().to_string() + ":" + std::to_string(_sender_endpoint.port()));
            log(LOG_INFO, "Received: " + msg);
            return msg;
        }

        // void handleRequest(std::error_code ec, std::size_t bytes_recvd) {
        //     if (!ec && bytes_recvd > 0) {
        //         std::string msg(_buffer.begin(), _buffer.begin() + bytes_recvd);
        //         log(LOG_INFO, "From: " + _sender_endpoint.address().to_string() + ":" + std::to_string(_sender_endpoint.port()));
        //         log(LOG_INFO, "Received: " + msg);
                
        //     }
        // }

        void log(int type, const std::string &msg) {
            _logs.emplace_back(type, msg);
            std::cout << msg << std::endl;
        };
    private:
        asio::io_context _io_context;
        udp::socket _socket;
        int _port;
        std::array<char, 1024> _buffer{};
        udp::endpoint _sender_endpoint;
        // mutex for socket endpoint
        std::mutex _socketMutex;
        // stop variable
        std::atomic<bool> _stopServer;
        std::vector<std::pair<int, std::string>> _logs;
        std::vector<Room *> *_rooms;
};