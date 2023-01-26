/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** structs.hpp by thibb1
*/

#ifndef RTYPE_STRUCTS_HPP
#define RTYPE_STRUCTS_HPP

#include "deps.hpp"

typedef struct {
    Vector3 position;
    float scale;
    Color color;
} Particle;

typedef struct {
    Texture2D texture;
    Vector2 scale;
    int columns;
    int rows;
} MyTexture;

typedef struct {
    int index;
    std::string name;
    Music music;
} MyMusic;

#endif // RTYPE_STRUCTS_HPP
