/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CObject.cpp
*/

#include "engine/components/CObject.hpp"

namespace Engine {
    void CObject::setTag(const std::string &tag) { _tags.insert(tag); }

    void CObject::removeTag(const std::string &tag) { _tags.erase(tag); }

    bool CObject::hasTag(const std::string &tag) const { return _tags.find(tag) != _tags.end(); }
}
