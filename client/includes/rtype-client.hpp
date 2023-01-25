/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main.hpp by thibb1
*/

#ifndef RTYPE_RTYPE_CLIENT_HPP
#define RTYPE_RTYPE_CLIENT_HPP
#define MAX_LIGHTS 100

#include "deps.hpp"
#include "light.hpp"
#include "structs.hpp"

void loadAssets(std::unordered_map<std::string, MyObject> &objects, std::unordered_map<std::string, MyMusic> &musics,
                std::unordered_map<std::string, Sound> &sounds, std::unordered_map<std::string, MyTexture> &textures,
                std::unordered_map<std::string, Shader> &shaders, const std::string &assetsPath);
BoundingBox GetMyObjectBoundingBox(const MyObject &myObject, Vector3 scale = Vector3Zero());
BoundingBox GetBoundingBoxAroundPoint(Vector3 position, float scale);
Slider CreateSlider(Rectangle bounds, float value, float minValue, float maxValue);
void UpdateSlider(Slider *slider);
void DrawSlider(Slider slider);
#endif // RTYPE_RTYPE_CLIENT_HPP
