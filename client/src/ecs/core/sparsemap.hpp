/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include <vector>
#include "ecs_traits.hpp"

namespace ecs {

template <typename Component>
class SparseMap {
private:
    static const size_t BLOCK_SIZE = 16;
    struct Block {
        Component dense[BLOCK_SIZE];
        Component *sparse[BLOCK_SIZE]{};
    };
public:
    SparseMap() : blocks({new Block()}), size(0) {}

    ~SparseMap() {
        for (auto block : this->blocks) {
            delete block;
        }
    }

    /**
     * @brief create a block containing his id and its value
     * @param id, value 
     */
    void insert(entity_type id, const Component &value) {    
        Block *block = this->blocks[id / BLOCK_SIZE];
        size_t index = id % BLOCK_SIZE;
        
        if (!block->sparse[index]) {
            block->sparse[index] = &this->blocks[size / BLOCK_SIZE]->dense[size % BLOCK_SIZE];
            *block->sparse[index] = value;
            ++size;
            if (size == blocks.size() * BLOCK_SIZE) {
                blocks.push_back(new Block());
            }
        } else {
            *block->sparse[index] = value;
        }
    }

    /**
     * @brief overriding opreator[]
     * @param id 
     * @return data of the block
     */
    Component &operator[](entity_type id) {
        Block *block = this->blocks[id / BLOCK_SIZE];
        size_t index = id % BLOCK_SIZE;

        return *block->sparse[index];
    }
private:
    std::vector<Block *> blocks;
    size_t size;
};

}
