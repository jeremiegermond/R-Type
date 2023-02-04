/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** utils.hpp by thibb1
*/

#ifndef RTYPE_UTILS_HPP
#define RTYPE_UTILS_HPP

#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define NB_R "(-?[0-9]+\\.?[0-9]*)"

std::vector<std::string> split(const std::string &s, char delim);

bool isMatch(const std::string &str, const std::string &pattern);
bool isNumber(const std::string &str);
std::string getMatch(const std::string &str, const std::string &pattern, int index = 0);
std::vector<std::string> getMatches(const std::string &str, const std::string &pattern);
#endif // RTYPE_UTILS_HPP
