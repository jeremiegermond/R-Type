/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** utils.cpp by thibb1
*/

#include "utils.hpp"

/**
 * It takes a string and a delimiter character, and returns a vector of strings, each of which is a
 * substring of the original string, delimited by the delimiter character
 * 
 * @param s The string to split
 * @param delim The delimiter character.
 * 
 * @return A vector of strings
 */
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
/**
 * It returns true if the string str matches the regular expression pattern
 * 
 * @param str The string to match against the pattern.
 * @param pattern The regular expression pattern to match against.
 * 
 * @return A boolean value.
 */
bool isMatch(const std::string &str, const std::string &pattern) {
    std::regex reg(pattern);
    return std::regex_match(str, reg);
}
/**
 * If the string is a number, return true, otherwise return false.
 * 
 * @param str The string to check
 * 
 * @return a boolean value.
 */
bool isNumber(const std::string &str) { return isMatch(str, "^[0-9]+$"); }

/**
 * It returns the first match of the given pattern in the given string
 * 
 * @param str The string to search in
 * @param pattern The regular expression pattern to match.
 * @param index The index of the match to return.
 * 
 * @return The string that matches the pattern.
 */
std::string getMatch(const std::string &str, const std::string &pattern, int index) {
    std::regex reg(pattern);
    std::smatch match;
    if (std::regex_search(str, match, reg)) {
        return match.str(index);
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
std::vector<std::string> getMatches(const std::string &str, const std::string &pattern) {
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
