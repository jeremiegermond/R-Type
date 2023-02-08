/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.cpp
*/

#include "engine/components/CAnimation.hpp"

namespace Engine {

    CAnimation::CAnimation() : _animations(nullptr), _animationCount(0), _animationIndex(0) {}

    CAnimation::~CAnimation() {
        if (_animations && _animationCount > 0)
            UnloadModelAnimations(_animations.get(), _animationCount);
    }

    ModelAnimation CAnimation::getAnimation() const {
        if (_animations && _animationCount > 0 && _animationIndex < _animationCount)
            return _animations.get()[_animationIndex];
        return {};
    }

    void CAnimation::setAnimation(const std::string &animationPath) {
        if (_animations && _animationCount > 0)
            UnloadModelAnimations(_animations.get(), _animationCount);
        auto animations = LoadModelAnimations(animationPath.c_str(), &_animationCount);
        _animations = std::make_shared<ModelAnimation>(*animations);
    }

    void Engine::CAnimation::setAnimationIndex(unsigned int index) {
        if (index < _animationCount)
            _animationIndex = index;
    }
}
