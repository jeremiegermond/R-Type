/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** deps.hpp by thibb1
*/

#ifndef RTYPE_DEPS_HPP
#define RTYPE_DEPS_HPP

#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
//include windwos headers here
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <set>
#include <thread>
#include <unordered_map>
#include <vector>

#endif // RTYPE_DEPS_HPP
