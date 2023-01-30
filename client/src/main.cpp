/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include "game/Game.hpp"

int main(int, char **) {
    Game game;
    game.Init();
    game.Update();
    game.Destroy();
    return 0;
}
