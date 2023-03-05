/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp
*/

#include "game/Game.hpp"

/**
 * @brief Parse the arguments
 * @param ac
 * @param av
 * @return std::pair<std::string, int>
 */
std::pair<std::string, unsigned short> parseArgs(int ac, char *av[]) {
    std::string ip = "127.0.0.1";
    int port = 4242;
    if (ac == 3 && Engine::Regex::isNumber(av[2])) {
        if (!Engine::Regex::isIp(av[1]))
            throw std::invalid_argument("Invalid IP");
        ip = av[1];
        port = std::stoi(av[2]);
    }
    return {ip, port};
}

int main(int ac, char *av[]) {
    try {
        auto [ip, port] = parseArgs(ac, av);
        Game game;
        game.init();
        game.initSocket(ip, port);
        game.update();
        game.destroy();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
