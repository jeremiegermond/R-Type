/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CObject.cpp
*/

#include "engine/components/CObject.hpp"

namespace Engine {
    /**
     * Adds a tag to the object.
     * 
     * @param tag The tag to be added to the object.
     */
    void CObject::setTag(const std::string &tag) { _tags.insert(tag); }

    /**
     * Remove a tag from the object.
     * 
     * @param tag The tag to remove from the object.
     */
    void CObject::removeTag(const std::string &tag) { _tags.erase(tag); }

    /**
     * If the tag is found in the set, return true, otherwise return false
     * 
     * @param tag The tag to check for.
     * 
     * @return A reference to the object's tag set.
     */
    bool CObject::hasTag(const std::string &tag) const { return _tags.find(tag) != _tags.end(); }
}
