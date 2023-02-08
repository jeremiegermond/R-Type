#include "network.hpp"

int main() {
    asio::io_context _io_context;
    UdpServer server;
    server.start();
    return 0;
}
