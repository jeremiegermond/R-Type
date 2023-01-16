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
        entity_type **newDense[BLOCK_SIZE];
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
     * @brief delete an entity corresponding to the given id
     * @param id 
     */
    void deleteEntity(entity_type id) {
        size_t block_index = id / BLOCK_SIZE;
        size_t index = id % BLOCK_SIZE;
        auto &block = blocks[block_index];

        // copy his value to the position of the deleted element
        block->dense[index] = block->dense[last_index];
        block->sparse[index] = &block->dense[index];
        block->sparse[last_index] = nullptr;
        --size;
        std::cout << "Entity deleted:" << id << std::endl;

        deleteBlockIfNeeded(id);
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

    void printSparseMap() {
        for (auto block : this->blocks) {
            for (size_t i = 0; i < BLOCK_SIZE; i++) {
                if (block->sparse[i]) {
                    std::cout << "index: " << i << ", sparse: " << *block->sparse[i] << ", dense: " << block->dense[i] << std::endl;
                }
            }
        }
    }
private:
    std::vector<Block *> blocks;
    size_t size;
    std::vector<size_t> entity_indices;
    
    /**
     * @brief delete a block containing his id if the last block is empty and if the penultimate is half or less of the block size
     * @param id 
     */
    void deleteBlockIfNeeded(entity_type id) {
        size_t block_index = id / BLOCK_SIZE;
        if (block_index >= blocks.size() || block_index == 0) return;
        auto &block = blocks[block_index];
        size_t penultimateBlock = 0;

        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            if (block->sparse[i]) {
                ++penultimateBlock;
            }
        }
        if (block_index == blocks.size() - 1 && (penultimateBlock <= BLOCK_SIZE / 2)) {
            std::cout << "Block deleted: " << block_index << std::endl;
            delete block;
            blocks.pop_back();
        }
    }
};

}
