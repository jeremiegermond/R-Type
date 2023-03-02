/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CTexture.cpp
*/

#include "engine/components/CTexture.hpp"

namespace Engine {
    CTexture::CTexture() : _texture(nullptr) {}

    CTexture::CTexture(const std::string &texturePath) : CTexture() { setTexture(texturePath); }

    void CTexture::doDestroy() {
        if (_texture) {
            UnloadTexture(*_texture);
            _texture.reset();
        }
        std::cout << "CTexture destroyed" << std::endl;
    }

    void CTexture::setTexture(const std::string &texturePath) {
        if (texturePath.empty())
            return;
        if (_texture)
            UnloadTexture(*_texture);
        if (!FileExists(texturePath.c_str()))
            return;
        _texture = std::make_shared<Texture2D>(LoadTexture(texturePath.c_str()));
        init();
    }
}
