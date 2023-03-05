/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** UdpClient.hpp
*/

#pragma once

#include "engine/CPPIncludes.hpp"

class UdpClient {
  private:
    asio::io_context _io_context;
    udp::socket _socket;
    udp::endpoint _endpoint;
    std::array<char, 1024> _buffer{};
    std::thread _listener;
    std::thread _heartbeat;
    std::thread _timeout;
    std::queue<std::string> _messages;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _connected;
    std::atomic<bool> _stop;
    std::chrono::time_point<std::chrono::steady_clock> _lastMessage;

  public:
    UdpClient(const std::string &ip, unsigned short port);
    ~UdpClient();

    // stop the client
    void stop();

    // start the client
    void start();

    // set the ip and port of the endpoint
    void setEndpoint(const std::string &ip, unsigned short port);

    // set the port of the endpoint
    void setPort(unsigned short port);

    // send a message to the server
    void send(const std::string &msg);

    // receive a message from the server
    std::string receive();

    // check if there is a message to receive
    bool hasMessage();

    // check if the client is connected to the server
    bool isConnected();

    // thread that listens for messages
    void threadListener();

    // thread that sends a heartbeat to the server
    void threadHeartbeat();

    // thread that checks if the server is still connected
    void threadTimeout();
};
