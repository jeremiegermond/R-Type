/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CObject.hpp by thibb1
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    class CObject : public ComponentBase {
        public:
            CObject() = default;

            void setTag(const std::string &tag);
            bool hasTag(const std::string &tag) const;

        private:
            std::unordered_set<std::string> _tags;
    };
}
