/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Regex.hpp
*/

#pragma once

#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace Engine {
// static functions
class Regex {
  public:
    /**
     * It returns true if the string str matches the regular expression pattern
     *
     * @param str The string to match against the pattern.
     * @param pattern The regular expression pattern to match against.
     *
     * @return A boolean value.
     */
    static bool isMatch(const std::string &str, const std::string &patern) {
        std::regex reg(patern);
        return std::regex_match(str, reg);
    }

    /**
     * If the string is a number, return true, otherwise return false.
     *
     * @param str The string to check
     *
     * @return a boolean value.
     */
    static bool isNumber(const std::string &str) { return isMatch(str, "^[0-9]+$"); }

    /**
     * If the string is an IP address, return true, otherwise return false.
     * @param str The string to check
     * @return a boolean value.
     */
    static bool isIp(const std::string &str) { return isMatch(str, "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$"); }

    /**
     * It returns the first match of the given pattern in the given string
     *
     * @param str The string to search in
     * @param pattern The regular expression pattern to match.
     * @param index The index of the match to return.
     *
     * @return The string that matches the pattern.
     */
    static std::string getMatch(const std::string &str, const std::string &pattern, int index = 0) {
        std::regex reg(pattern);
        std::smatch match;
        if (std::regex_search(str, match, reg)) {
            return match[index];
        }
        return "";
    }

    /**
     * It takes a string and a pattern, and returns a vector of strings that match the pattern
     *
     * @param str The string to search in
     * @param pattern The regular expression pattern to match.
     *
     * @return A vector of strings.
     */
    static std::vector<std::string> getMatches(const std::string &str, const std::string &pattern) {
        std::regex reg(pattern);
        std::smatch match;
        std::vector<std::string> matches;
        if (std::regex_search(str, match, reg)) {
            for (auto &m : match) {
                matches.push_back(m.str());
            }
        }
        return matches;
    }
};
} // namespace Engine
