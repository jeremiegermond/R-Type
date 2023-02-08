#include "network.hpp"

/**
 * It creates a UDP socket, binds it to a random port, and starts three threads: one to listen for
 * incoming messages, one to send heartbeat messages, and one to check for timeouts
 * 
 * @param ip the ip address of the server
 * @param port The port to listen on.
 */
UdpClient::UdpClient(const std::string &ip, unsigned short port)
    : _socket(_io_context, udp::endpoint(udp::v4(), 0)), _endpoint(address::from_string(ip), port) {
    std::cout << "Udp client started" << std::endl;
    _listener = std::thread([this]() { threadListener(); });
    _listener.detach();
    _heartbeat = std::thread([this]() { threadHeartbeat(); });
    _heartbeat.detach();
    _timeout = std::thread([this]() { threadTimeout(); });
    _timeout.detach();
    std::cout << "Udp client started" << std::endl;
}

/**
 * It closes the socket, stops the io_context, and prints a message to the console
 */
UdpClient::~UdpClient() {
    send("disconnect");
    _socket.close();
    _io_context.stop();
    std::cout << "Udp client stopped" << std::endl;
}

/**
 * It sends a message to the server
 * 
 * @param msg The message to send.
 */
void UdpClient::send(const std::string &msg) {
    std::error_code ec;
    _socket.send_to(asio::buffer(msg), _endpoint, 0, ec);
    if (ec) {
        std::cerr << "Error while sending message: " << ec.message() << std::endl;
    }
}

/**
 * It waits until there's a message in the queue, then returns it
 * 
 * @return A string
 */
std::string UdpClient::receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() { return !_messages.empty(); });
    std::string msg = _messages.front();
    _messages.pop();
    return msg;
}

/**
 * If the messages queue is not empty, return true.
 * 
 * @return A boolean value.
 */
bool UdpClient::hasMessage() {
    std::lock_guard<std::mutex> lock(_mutex);
    return !_messages.empty();
}

/**
 * It returns the value of the private variable _connected.
 * 
 * @return A boolean value.
 */
bool UdpClient::isConnected() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _connected;
}

/**
 * It listens for incoming messages and stores them in a queue
 */
void UdpClient::threadListener() {
    while (true) {
        std::error_code ec;
        std::size_t bytes_recvd = _socket.receive_from(asio::buffer(_buffer), _endpoint, 0, ec);
        if (ec == asio::error::operation_aborted || ec == asio::error::bad_descriptor) {
            break;
        }
        if (ec) {
            std::cerr << "Error while receiving message: " << ec.message() << std::endl;
            _connected = false;
            break;
        }
        std::string msg(_buffer.data(), bytes_recvd);
        std::lock_guard<std::mutex> lock(_mutex);
        _messages.push(msg);
        _lastMessage = std::chrono::steady_clock::now();
        _connected = true;
        _cv.notify_one();
    }
}

/**
 * It sends a "ping" message to the server every 3 seconds
 */
void UdpClient::threadHeartbeat() {
    while (true) {
        send("ping");
        std::this_thread::sleep_for(3s);
    }
}

/**
 * "If the last message was received more than 3 seconds ago, set the connected flag to false."
 * 
 * The first thing we do is sleep for 5 seconds. This is to prevent the thread from constantly checking
 * the time
 */
void UdpClient::threadTimeout() {
    while (true) {
        std::this_thread::sleep_for(5s);
        if (std::chrono::steady_clock::now() - _lastMessage > 3s) {
            std::cout << "Server timed out" << std::endl;
            std::lock_guard<std::mutex> lock(_mutex);
            _connected = false;
        }
    }
}
