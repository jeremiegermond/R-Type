/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include "sparsemap.hpp"
#include <utility>
#include <tuple>
#include <functional>

namespace ecs {

template <typename... Components>
class Archetype {
public:
    Archetype() : nextEntityId(0) {}

    /**
     * @brief create an entity
     * @param values
     * @return id of this entity
     */ 
    template <typename... Components>
    entity_type create_entity(Components&&... values) {
        (insert(next_entity_id, std::forward<Components>(values)), ...);
        return next_entity_id++;
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

private:
    entity_type nextEntityId;
    std::tuple<SparseMap<Components>...> componentMaps;

    template <typename Component>
    void insert(entity_type id, Component &&value) {
        SparseMap<Component> &map = std::get<SparseMap<Component>>(componentMaps);
        map.insert(id, std::forward<Component>(value));
    }
};

}