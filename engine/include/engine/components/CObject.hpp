/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CObject.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
  /**
 * @brief Class used to handle CObject
 * 
 */
    class CObject : public ComponentBase {
      public:
        CObject() = default;

        void setTag(const std::string &tag);
        void removeTag(const std::string &tag);
        bool hasTag(const std::string &tag) const;

      private:
        std::unordered_set<std::string> _tags;
    };
}
