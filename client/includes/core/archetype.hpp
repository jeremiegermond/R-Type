/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#include "sparsemap.hpp"
#include <functional>
#include <set>
#include <tuple>
#include <utility>

namespace ecs {

template <typename... Components>
class Archetype {
  public:
    Archetype() : nextEntityId(0) {}

    /**
     * @brief create an entity
     * @param values of the component
     * @return id of this entity
     */
    template <typename... Component>
    entity_type create_entity(Component &&...values) {
        (insert_component(nextEntityId, std::forward<Component>(values)), ...);
        return nextEntityId++;
    }

    /**
     * @brief delete an entity
     * @param id of the entity
     */
    template <typename... Component>
    void delete_entity(entity_type id) {
        if (deletedEntities.count(id) > 0)
            return;
        deletedEntities.insert(id);
        (delete_component<Component>(id), ...);
    }

    template <typename Component>
    void insert_component(entity_type id, Component &&value) {
        SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
        map.insert(id, std::forward<Component>(value));
    }

    template <typename Component>
    void delete_component(entity_type id) {
        SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
        map.erase(id);
    }

    /**
     * @brief query entities components
     * @param id
     * @return If you chose to query 1 Component, this function returns the reference to that component
     * otherwise it returns a tuple of references to specified components
     */
    template <size_t... Ids>
    decltype(auto) query(entity_type id) {
        if constexpr (sizeof...(Ids) == 1) {
            return std::get<Ids...>(componentMaps)[id];
        } else {
            return std::forward_as_tuple(std::get<Ids>(componentMaps)[id]...);
        }
    }

    /**
     * @brief get deleted entities
     * @param id
     * @return deleted entites
     */
    std::set<entity_type> &getDeletedEntities() { return deletedEntities; }

  private:
    entity_type nextEntityId;
    std::tuple<SparseMap<Components>...> componentMaps;
    std::set<entity_type> deletedEntities;
};

} // namespace ecs
