/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#pragma once

#include "engine/CPPIncludes.hpp"

typedef unsigned int EntityId;

namespace Engine {
    template<typename... Components>
    class Archetype {
    private:
        std::unordered_map<EntityId, std::tuple<Components...>> _entities;
        std::set<EntityId> _removedEntities;
        EntityId _nextId = 0;
    public:
        Archetype() = default;
        ~Archetype() = default;

        /**
         * @brief add an entity to the archetype
         * @param components of the entity
         */
        EntityId createEntity(Components... components) {
            EntityId id;
            if (_removedEntities.empty()) {
                id = _nextId++;
            } else {
                id = *_removedEntities.begin();
                _removedEntities.erase(_removedEntities.begin());
            }
            _entities[id] = std::make_tuple(components...);
            return id;
        }

        /**
         * @brief remove an entity from the archetype
         * @param id of the entity
         */
        void removeEntity(EntityId id) {
            _entities.erase(id);
            _removedEntities.insert(id);
        }

        /**
         * @brief get the components of an entity
         * @param id of the entity
         * @return components of the entity
         */
        template<typename ...Args>
        decltype(auto) getComponent(EntityId id) {
            if constexpr (sizeof...(Args) == 1) {
                return std::get<Args...>(_entities[id]);
            } else {
                return std::tie(std::get<Args>(_entities[id])...);
            }
        }

        /**
         * @brief get the size of the archetype
         * @return size of the archetype
         */
        size_t size() {
            return _entities.size();
        }

        /**
         * @brief delete all the entities in the archetype
         */
        void clear() {
            _entities.clear();
        }
    };
}
