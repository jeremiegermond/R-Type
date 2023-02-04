#pragma once

#include "deps.hpp"
#include "utils.hpp"

#include <arpa/inet.h>
#include <array>
#include <asio.hpp>
#include <cstring>
#include <functional>
#include <future>
#include <iomanip>
#include <netinet/in.h>
#include <poll.h>
#include <queue>
#include <random>
#include <string>
#include <utility>

using namespace std::chrono_literals;
using namespace asio::ip;

// udp client that can send and receive data
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
    std::chrono::time_point<std::chrono::system_clock> _lastMessage;

  public:
    UdpClient(const std::string &ip, unsigned short port);
    ~UdpClient();

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
