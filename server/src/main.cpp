/*
** EPITECH PROJECT, 2023
** server
** File description:
** main.cpp by thibb1
*/

#include "network.hpp"

int main(int, char *[]) {
    try {
        asio::io_context io_context;
        UdpServer server(io_context, 12345);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
