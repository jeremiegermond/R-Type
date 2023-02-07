/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CSprite.hpp by thibb1
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    class CSprite : public ComponentBase {
        public:
            CSprite();

            explicit CSprite(const std::string &texturePath);

            void doDestroy() override;

            /**
             * @brief Set the Texture object
             * @param texturePath
             * @details if the texturePath is empty, it will do nothing
             */
            void setTexture(const std::string &texturePath);
            /**
             * @brief Get the Texture object
             * @return std::shared_ptr<Texture2D>
             * @note if the texture is not set, it will return a nullptr
             */
            [[nodiscard]] std::shared_ptr<Texture2D> getTexture() const { return _texture; }

        private:
            std::shared_ptr<Texture2D> _texture;
    };
}
