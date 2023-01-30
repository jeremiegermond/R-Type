/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#pragma once

#include "Sparsemap.hpp"
#include <functional>
#include <set>
#include <tuple>
#include <utility>

namespace Engine {

template <typename... Components>
class Archetype {
  public:
    Archetype() : nextEntityId(0), componentMaps(SparseMap<Components>()...), deletedEntities() {}

    virtual ~Archetype() { Clear(); }

    /**
     * @brief delete all entities
     */
    void Clear() {
        (std::get<SparseMap<Components>>(componentMaps).Clear(), ...);
        // deletedEntities.clear();
        nextEntityId = 0;
    }

    /**
     * @brief create an entity
     * @param values of the component
     * @return id of this entity
     */
    template <typename... Component>
    EntityId CreateEntity(Component &&...values) {
        (InsertComponent(nextEntityId, std::forward<Component>(values)), ...);
        return nextEntityId++;
    }

    /**
     * @brief delete an entity
     * @param id of the entity
     */
    template <typename... Component>
    void DeleteEntity(EntityId id) {
        if (deletedEntities.count(id) > 0)
            return;
        deletedEntities.insert(id);
        (DeleteComponent<Component>(id), ...);
    }

    template <typename Component>
    void InsertComponent(EntityId id, Component &&value) {
        SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
        map.Insert(id, std::forward<Component>(value));
    }

    template <typename Component>
    void DeleteComponent(EntityId id) {
        SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
        map.Erase(id);
    }

    /**
     * @brief Query entities components via their id
     * @param id of the entity
     * @return If you chose to Query 1 ComponentBase, this function returns the reference to that component
     * otherwise it returns a tuple of references to specified components
     */
    template <size_t... Ids>
    decltype(auto) Query(EntityId id) {
        if constexpr (sizeof...(Ids) == 1) {
            return std::get<Ids...>(componentMaps)[id];
        } else {
            return std::forward_as_tuple(std::get<Ids>(componentMaps)[id]...);
        }
    }

    /**
     * @brief Query entities components via their type
     * @param id of the entity
     * @return If you chose to Query 1 ComponentBase, this function returns the reference to that component
     * otherwise it returns a tuple of references to specified components
     */
    template <typename... Component>
    decltype(auto) Query(EntityId id) {
        if constexpr (sizeof...(Component) == 1) {
            return std::get<SparseMap<Component...>>(componentMaps)[id];
        } else {
            return std::forward_as_tuple(std::get<SparseMap<Component>>(componentMaps)[id]...);
        }
    }

    /**
     * @brief get deleted entities
     * @param id
     * @return deleted entites
     */
    std::set<EntityId> &GetDeletedEntities() { return deletedEntities; }

  private:
    EntityId nextEntityId;
    std::tuple<SparseMap<Components>...> componentMaps;
    std::set<EntityId> deletedEntities;
};

} // namespace Engine
