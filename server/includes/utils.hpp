/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** utils.hpp by thibb1
*/

#pragma once

#include <asio.hpp>
#include <iostream>
#include <map>
#include <random>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"

#define NB_R "(-?[0-9]+\\.?[0-9]*)"

std::vector<std::string> split(const std::string &s, char delim);
std::string floatToString(float number, int precision = 1);
std::string getPositionString(Vector2 position);

bool isMatch(const std::string &str, const std::string &pattern);
bool isNumber(const std::string &str);
std::string getMatch(const std::string &str, const std::string &pattern, int index = 0);
std::vector<std::string> getMatches(const std::string &str, const std::string &pattern);

class idGenerator {
  private:
    std::vector<unsigned int> ids;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

  public:
    // Clang-Tidy: Random number generator seeded with a default argument will generate a predictable sequence of values
    idGenerator() {
        // Warning: this is not a good way to seed the generator
        std::random_device rd;
        gen = std::mt19937(rd());
        // This is the good way to seed the generator:
        // std::random_device rd;
        // std::mt19937 gen(rd());

        dis = std::uniform_int_distribution<>(0, 1000000);
        for (int i = 0; i < 1000000; i++) {
            ids.push_back(dis(gen));
        }
    }
    unsigned int getUniqueId() {
        unsigned int id = ids.back();
        ids.pop_back();
        return id;
    }
};
