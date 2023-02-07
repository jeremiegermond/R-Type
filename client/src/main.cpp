/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "game/Game.hpp"

int main(int, char **) {
    Game game;
    game.init();
    game.update();
    game.destroy();
    return 0;
}
