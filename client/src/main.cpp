/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.cpp by thibb1
*/

#include <iostream>
#include <raylib.h>
#include "Main.hpp"

int main()
{
    try {
        rtype::Game game;

        game.createWindow();
        game.run();
    } catch (...) {}
    return 0;
}
