/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#include <cstdint>
#include <limits>

namespace ecs {
using entity_type = uint32_t;
const entity_type NULL_ENTITY = std::numeric_limits<entity_type>::max();
} // namespace ecs
