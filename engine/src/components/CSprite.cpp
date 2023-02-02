/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CSprite.cpp by thibb1
*/

#include "engine/components/CSprite.hpp"

namespace Engine {
    CSprite::CSprite() : _texture(nullptr) {}

    CSprite::CSprite(const std::string &texturePath) : CSprite() { SetTexture(texturePath); }

    void CSprite::DoDestroy() {
        if (_texture) {
            UnloadTexture(*_texture);
            _texture.reset();
        }
        std::cout << "CSprite destroyed" << std::endl;
    }

    void CSprite::SetTexture(const std::string &texturePath) {
        if (texturePath.empty())
            return;
        if (_texture)
            UnloadTexture(*_texture);
        if (!FileExists(texturePath.c_str()))
            return;
        _texture = std::make_shared<Texture2D>(LoadTexture(texturePath.c_str()));
        Init();
    }
}
