/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#pragma once

#include "engine/manager/Sparsemap.hpp"

namespace Engine {
    template <typename... Components>
    class Archetype {
      public:
        Archetype() : nextEntityId(0), componentMaps(SparseMap<Components>()...), deletedEntities() {}

        /**
         * @brief create an entity
         * @param values of the component
         * @return id of this entity
         */
        template <typename... Component>
        EntityId createEntity(Component &&...values) {
            (addComponent(nextEntityId, std::forward<Component>(values)), ...);
            return nextEntityId++;
        }

        /**
         * @brief delete an entity
         * @param id of the entity
         */
        template <typename... Component>
        void deleteComponent(EntityId id) {
            if (deletedEntities.count(id) > 0)
                return;
            deletedEntities.insert(id);
            (deleteComponent<Component>(id), ...);
        }

        template <typename Component>
        void addComponent(EntityId id, Component &&value) {
            SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
            map.insert(id, std::forward<Component>(value));
        }

        template <typename Component>
        void deleteEntity(EntityId id) {
            SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
            map.erase(id);
        }

        /**
         * @brief getComponent entities components via their id
         * @param id of the entity
         * @return If you chose to getComponent 1 ComponentBase, this function returns the reference to that component
         * otherwise it returns a tuple of references to specified components
         */
        template <size_t... Ids>
        decltype(auto) getComponent(EntityId id) {
            if constexpr (sizeof...(Ids) == 1) {
                return std::get<Ids...>(componentMaps)[id];
            } else {
                return std::forward_as_tuple(std::get<Ids>(componentMaps)[id]...);
            }
        }

        /**
         * @brief getComponent entities components via their type
         * @param id of the entity
         * @return If you chose to getComponent 1 ComponentBase, this function returns the reference to that component
         * otherwise it returns a tuple of references to specified components
         */
        template <typename... Component>
        decltype(auto) getComponent(EntityId id) {
            if constexpr (sizeof...(Component) == 1) {
                return std::get<SparseMap<Component...>>(componentMaps)[id];
            } else {
                return std::forward_as_tuple(std::get<SparseMap<Component>>(componentMaps)[id]...);
            }
        }

        std::set<EntityId> &getDeletedEntities() { return deletedEntities; }

      private:
        EntityId nextEntityId;
        std::tuple<SparseMap<Components>...> componentMaps;
        std::set<EntityId> deletedEntities;
    };
}
