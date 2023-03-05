/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CTexture.cpp
*/

#include "engine/components/CTexture.hpp"

namespace Engine {
    /**
     * CTexture::CTexture() : _texture(nullptr) {}
     */
    CTexture::CTexture() : _texture(nullptr) {}

    /**
     * CTexture::CTexture(const std::string &texturePath) : CTexture() { setTexture(texturePath); }
     * 
     * @param texturePath The path to the texture file.
     */
    CTexture::CTexture(const std::string &texturePath) : CTexture() { setTexture(texturePath); }

    /**
     * If the texture is loaded, unload it and reset the pointer
     */
    void CTexture::doDestroy() {
        if (_texture) {
            UnloadTexture(*_texture);
            _texture.reset();
        }
        std::cout << "CTexture destroyed" << std::endl;
    }

    /**
     * If the texturePath is empty, return. If the texture is not null, unload it. If the texturePath
     * does not exist, return. If the texture is null, load it
     * 
     * @param texturePath The path to the texture file.
     * 
     * @return A pointer to a Texture2D object.
     */
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
    /**
     * This function sets the number of rows and columns in the texture
     * 
     * @param rows The number of rows in the texture.
     * @param columns The number of columns in the texture.
     */
    void CTexture::setRowsAndColumns(int rows, int columns) {
        _rows = rows;
        _columns = columns;
    }
    /**
     * This function returns the number of rows in the texture.
     * 
     * @return The number of rows in the texture.
     */
    int CTexture::getRows() const { return _rows; }
    /**
     * Returns the number of columns in the texture
     * 
     * @return The number of columns in the texture.
     */
    int CTexture::getColumns() const { return _columns; }
    /**
     * Sets the scale of the texture.
     * 
     * @param scale The scale of the texture.
     */
    void CTexture::setScale(const Vector2 &scale) { _scale = scale; }
    /**
     * Returns the scale of the texture.
     * 
     * @return The scale of the texture.
     */
    Vector2 CTexture::getScale() const { return _scale; }
}
