/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** HashId.hpp by thibb1
*/

#pragma once

#include <cstddef>
#include <typeinfo>

namespace Engine {
/**
 * @brief HashId is a class that allows to hash a template type
 */
class HashId {
  public:
    /**
     * @brief Get the hash of a template type
     * @tparam T
     * @return size_t
     * @details This function is used to get the hash of a template type
     */
    template <typename T>
    static size_t Get() {
        static size_t hash = typeid(T).hash_code();
        return hash;
    }
};
} // namespace Engine
