#pragma once

#include "utils.hpp"
#include "room.hpp"

using asio::ip::tcp;

class EngineUdpServer {
    public:
        EngineUdpServer(int port) : _socket(_io_context, udp::endpoint(udp::v4(), port)) {
            _port = port;
        }
        EngineUdpServer(int port, std::vector<std::pair<int, std::string>> &output) : _socket(_io_context, udp::endpoint(udp::v4(), port)) {
            _port = port;
            _output = &output;
        }

        ~EngineUdpServer() {
            _stopServer = true;
            _io_context.stop();
        }

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
                if (*room == port) {// create exception
                    return;
                }
            _rooms->push_back(new Room(port));
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

        void log(int type, const std::string &msg) {
            if (_output)
                _output->emplace_back(type, msg);
            else
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
        std::vector<Room *> *_rooms;
        std::vector<std::pair<int, std::string>> *_output;
};