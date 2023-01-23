/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** 
*/

#include <vector>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include "ecs_traits.hpp"

namespace ecs {

template <typename Component>
class SparseMap {
private:
    static const size_t BLOCK_SIZE = 16;
    struct Block {
        Component components[BLOCK_SIZE];
        entity_type sparse[BLOCK_SIZE];
        entity_type dense[BLOCK_SIZE];
    };
public:
    SparseMap() : blocks({new Block()}), size(0) {}

    ~SparseMap() {
        for (auto block : this->blocks) {
            delete block;
        }
    }

    void insert(entity_type id, const Component &value) {
        size_t sparseIndex = inBlockIndex(id);

        if (id == size && id == blocks.size() * BLOCK_SIZE) {
            blocks.push_back(new Block());
        }
        Block *block = this->blocks[blockIndex(id)];

        if (!block->sparse[sparseIndex]) {
            this->max = std::max(id, this->max);
            this->blocks[blockIndex(this->size)]->dense[inBlockIndex(this->size)] = id;
            this->blocks[blockIndex(this->size)]->components[inBlockIndex(this->size)] = value;
            block->sparse[sparseIndex] = this->size;
            ++size;
        } else {
            this->blocks[blockIndex(block->sparse[sparseIndex])]->components[inBlockIndex(block->sparse[sparseIndex])] = value;
        }
    }

    void erase(entity_type id) {
        size_t index = inBlockIndex(id);
        Block *block = this->blocks[blockIndex(id)];
        entity_type previousSparseValue = block->sparse[index];

        if (id == this->max) {
            updateMax();
            if (blocks.size() > 1 && max < BLOCK_SIZE / 2 + (blocks.size() - 2) * BLOCK_SIZE) {
                delete this->blocks[this->blocks.size() - 1];
                this->blocks.pop_back();
            }
        }

        if (previousSparseValue != NULL_ENTITY) {
            block->sparse[index] = NULL_ENTITY;
        }

        block->sparse[index] = block->dense[index];
        entity_type *lastDensePtr = &this->blocks[blockIndex(this->size - 1)]->dense[inBlockIndex(this->size - 1)];
        if (*lastDensePtr == id) return;
        block->dense[index] = *lastDensePtr;
        *lastDensePtr = id;
        block->sparse[inBlockIndex(*lastDensePtr)] = index;
        --size;
    }

    Component &operator[](entity_type id) {
        Block *block = this->blocks[blockIndex(id)];
        size_t denseIndex = block->sparse[inBlockIndex(id)];
        return block->components[denseIndex];
    }

private:
    std::vector<Block *> blocks;
    size_t size;
    entity_type max;

    size_t inBlockIndex(entity_type id) {
        return id % BLOCK_SIZE;
    }

    size_t blockIndex(entity_type id) {
        return id / BLOCK_SIZE;
    }

    void updateMax() {
        for (auto i = max; i > 0; --i) {
            if (blocks[blockIndex(i - 1)]->sparse[inBlockIndex(i - 1)] != NULL_ENTITY) {
                this->max = i - 1;
                return;
            }
        }
        this->max = 0;
    }
};

}
