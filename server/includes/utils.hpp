/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** utils.hpp by thibb1
*/

#pragma once

#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
//include windwos headers here
#include "raylib.h"
#include "raymath.h"

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
#endif

#include <asio.hpp>
#include <iostream>
#include <map>
#include <random>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>


using namespace std::chrono;

#define NB_R "(-?[0-9]+\\.?[0-9]*)"

std::vector<std::string> split(const std::string &s, char delim);
std::string floatToString(float number, int precision = 1);
std::string vectorToString(Vector2 vector2);

bool isMatch(const std::string &str, const std::string &pattern);
bool isNumber(const std::string &str);
std::string getMatch(const std::string &str, const std::string &pattern, int index = 0);
std::vector<std::string> getMatches(const std::string &str, const std::string &pattern);

BoundingBox GetBoundingBoxAroundPoint(Vector2 vector2, float scale);
