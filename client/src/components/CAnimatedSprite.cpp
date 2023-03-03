/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimatedSprite.cpp
*/

#include "components/CAnimatedSprite.hpp"

CAnimatedSprite::CAnimatedSprite()
    : _scale({1, 1}), _sourceRec({0, 0, 0, 0}), _rows(0), _columns(0), _currentFrame(0), _enabled(true), _totalFrames(0), _width(0), _height(0) {}

CAnimatedSprite::~CAnimatedSprite() = default;

void CAnimatedSprite::doUpdate() {
    _currentFrame++;
    if (_currentFrame >= _totalFrames) {
        _enabled = false;
    }
    _sourceRec.x = float((_currentFrame % _columns) * _width);
    _sourceRec.y = (_currentFrame / _rows) * _height;
}

void CAnimatedSprite::setTextureSize(Vector2 size, int rows, int columns) {
    _width = size.x / columns;
    _height = size.y / rows;
    _rows = rows;
    _columns = columns;
    _sourceRec = {0, 0, float(_width), float(_height)};
    _totalFrames = _rows * _columns;
}

Rectangle CAnimatedSprite::getSourceRec() const { return _sourceRec; }

void CAnimatedSprite::setScale(Vector2 scale) { _scale = scale; }

Vector2 CAnimatedSprite::getScale() const { return _scale; }

bool CAnimatedSprite::getEnabled() const { return _enabled; }

void CAnimatedSprite::setRotation(float rotation) { _rotation = rotation; }

float CAnimatedSprite::getRotation() const { return _rotation; }
