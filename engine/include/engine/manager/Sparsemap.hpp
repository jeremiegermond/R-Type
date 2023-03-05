/*
** EPITECH PROJECT, 2023
** rtype
** File description:
**
*/

#pragma once

#include "engine/utils/Defines.hpp"

namespace Engine {
    template <typename Component>
    /**
 * @brief Class used for the SparseMap used in the ECS
 * 
 */
    class SparseMap {
      private:
        static const size_t BLOCK_SIZE = 1600000;
        struct Block {
            Component components[BLOCK_SIZE];
            EntityId sparse[BLOCK_SIZE];
            EntityId dense[BLOCK_SIZE];
        };
        std::vector<Block *> _blocks;
        size_t _size;
        EntityId _max{};

        /**
         * @brief get the index in the block
         * @param id of the entity
         * @return index in the block
         */
        size_t inBlockIndex(EntityId id) { return id % BLOCK_SIZE; }

        /**
         * @brief get the block index
         * @param id of the entity
         * @return index of the block
         */
        size_t blockIndex(EntityId id) { return id / BLOCK_SIZE; }

        /**
         * @brief update the max value
         */
        void updateMax() {
            for (auto i = _max; i > 0; --i) {
                if (_blocks[blockIndex(i - 1)]->sparse[inBlockIndex(i - 1)] != NULL_ENTITY) {
                    this->_max = i - 1;
                    return;
                }
            }
            this->_max = 0;
        }

      public:
        SparseMap() : _blocks({new Block()}), _size(0) {}

        ~SparseMap() { clear(); }

        /**
         * @brief create a block containing his id and its value
         * @param id, value
         */
        void insert(EntityId id, const Component &value) {
            size_t sparseIndex = inBlockIndex(id);

            if (id == _size && id == _blocks.size() * BLOCK_SIZE) {
                _blocks.push_back(new Block());
            }
            Block *block = this->_blocks[blockIndex(id)];

            if (!block->sparse[sparseIndex]) {
                this->_max = std::max(id, this->_max);
                this->_blocks[blockIndex(this->_size)]->dense[inBlockIndex(this->_size)] = id;
                this->_blocks[blockIndex(this->_size)]->components[inBlockIndex(this->_size)] = value;
                block->sparse[sparseIndex] = this->_size;
                ++_size;
            } else {
                this->_blocks[blockIndex(block->sparse[sparseIndex])]->components[inBlockIndex(block->sparse[sparseIndex])] = value;
            }
        }

        /**
         * @brief erase an entity corresponding to the given id
         * @param id
         */
        void erase(EntityId id) {
            size_t index = inBlockIndex(id);
            Block *block = this->_blocks[blockIndex(id)];
            EntityId previousSparseValue = block->sparse[index];

            if (id == this->_max) {
                updateMax();
                if (_blocks.size() > 1 && _max < BLOCK_SIZE / 2 + (_blocks.size() - 2) * BLOCK_SIZE) {
                    delete this->_blocks[this->_blocks.size() - 1];
                    this->_blocks.pop_back();
                }
            }

            if (previousSparseValue != NULL_ENTITY) {
                block->sparse[index] = NULL_ENTITY;
            }

            block->sparse[index] = block->dense[index];
            EntityId *lastDensePtr = &this->_blocks[blockIndex(this->_size - 1)]->dense[inBlockIndex(this->_size - 1)];
            if (*lastDensePtr == id)
                return;
            block->dense[index] = *lastDensePtr;
            *lastDensePtr = id;
            block->sparse[inBlockIndex(*lastDensePtr)] = index;
            --_size;
        }

        /**
         * @brief clear the SparseMap
         */
        void clear() {
            for (auto block : this->_blocks) {
                delete block;
            }
            this->_blocks.clear();
            this->_blocks.push_back(new Block());
            this->_size = 0;
            this->_max = 0;
        }

        /**
         * @brief overriding opreator[]
         * @param id
         * @return data of the block
         */
        Component &operator[](EntityId id) {
            Block *block = this->_blocks[blockIndex(id)];
            size_t denseIndex = block->sparse[inBlockIndex(id)];
            return block->components[denseIndex];
        }
    };

}
