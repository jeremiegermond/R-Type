/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CPPIncludes.hpp
*/

#pragma once

#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <algorithm>
#include <any>
#include <asio.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std::chrono_literals;
using namespace asio::ip;
