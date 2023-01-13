/*
** EPITECH PROJECT, 2023
** game.hpp
** File description:
** TODO
*/

#ifndef RTYPE_GAME_HPP
#define RTYPE_GAME_HPP

#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include "raylib.h"
#include "SceneManager.hpp"


namespace rtype {
    class Game {
    private:
        int width;
        int height;
        int currentScene;
        SceneManager scenes;
        Image flavicon;
    
    public:
        Game();
        ~Game();
        void createWindow();
        void run();
        void ChangeScene(int i);
        Scene *GetScene();
        SceneManager GetSceneManager();
        void CloseGame();
    };

}
#endif