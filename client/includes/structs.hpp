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
} Particle;

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float scale;
    int lightIndex;
    std::shared_ptr<Light> light;
} Bullet;

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 rotationGoal;
    float scale;
    ModelAnimation *animations;
    unsigned int animationCount;
    int currentAnimation;
    int currentFrame;
    Model model;
    std::vector<Texture2D *> textures;
} MyObject;

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

typedef struct {
    MyTexture *texture;
    Vector2 position;
    int totalFrames;
    int currentFrame;
    int columns;
    int rows;
    int width;
    int height;
    float rotation;
} Particle2D;

// typedef struct {
//     Rectangle bounds;
//     float *value;
//     float minValue;
//     float maxValue;
//     bool dragging;
//     bool enabled;
//     Color baseColor;
//     Color selectedColor;
//     Color disabledColor;
// } Slider;

#endif // RTYPE_STRUCTS_HPP
