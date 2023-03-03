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

        void start(std::vector<Room *> &rooms) {
            log(LOG_INFO, "UDP server started on port " + std::to_string(_port));
            _rooms = &rooms;
            receiveRequest();
            _io_context.run();
        }

        void receiveRequest() {
            if (_stopServer)
                return;
            _socket.async_receive_from(asio::buffer(_buffer), _sender_endpoint, [this](std::error_code ec, std::size_t bytes_recvd) {
                handleRequest(ec, bytes_recvd);
                receiveRequest();
            });
        }

        void sendResponse(const udp::endpoint &endpoint, const std::string &msg) {
            std::lock_guard<std::mutex> lock(_socketMutex);
            _socket.send_to(asio::buffer(msg), endpoint);
        }

        void handleRequest(std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::string msg(_buffer.begin(), _buffer.begin() + bytes_recvd);
                log(LOG_INFO, "From: " + _sender_endpoint.address().to_string() + ":" + std::to_string(_sender_endpoint.port()));
                log(LOG_INFO, "Received: " + msg);
                std::string buffer;
                for (auto room : *_rooms) {
                    buffer += std::to_string(room->get_port()) + ":" + std::to_string(room->get_player_count()) + ',';
                }
                buffer.pop_back();
                sendResponse(_sender_endpoint, buffer);  // send list of rooms ports and infos
            }
        }

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