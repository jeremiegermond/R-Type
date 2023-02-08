/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.cpp
*/

#include "engine/components/CAnimation.hpp"

namespace Engine {

    CAnimation::CAnimation() : _animations(nullptr), _animationCount(0), _animationIndex(0) {}

    CAnimation::~CAnimation() { unloadAnimations(); }

    void CAnimation::unloadAnimations() {
        if (_animations && _animationCount > 0) {
            UnloadModelAnimations(_animations, _animationCount);
            _animations = nullptr;
            _animationCount = 0;
            _animationIndex = 0;
        }
    }

    ModelAnimation CAnimation::getAnimation() const {
        if (_animations && _animationCount > 0 && _animationIndex < _animationCount)
            return _animations[_animationIndex];
        return {};
    }

    void CAnimation::setAnimation(const std::string &animationPath) {
        unloadAnimations();
        if (!FileExists(animationPath.c_str()))
            throw std::runtime_error("Animation file not found: " + animationPath);
        _animations = LoadModelAnimations(animationPath.c_str(), &_animationCount);
    }

    void Engine::CAnimation::setAnimationIndex(unsigned int index) {
        if (index < _animationCount)
            _animationIndex = index;
    }
}
