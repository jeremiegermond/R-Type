/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.cpp
*/

#include "engine/components/CAnimation.hpp"

namespace Engine {

    CAnimation::CAnimation() : _animations(nullptr), _animationCount(0), _animationIndex(0), _currentFrame(0), _lastUpdate(GetTime()) {}

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

    void CAnimation::setAnimationIndex(unsigned int index) {
        if (index < _animationCount) {
            _animationIndex = index;
            _currentFrame = 0;
        }
    }

    int CAnimation::getCurrentFrame() const { return _currentFrame; }

    void CAnimation::doUpdate() {
        if (GetTime() - _lastUpdate > 0.04) {
            _currentFrame++;
            if (_animations && _animationCount > 0 && _animationIndex < _animationCount && _currentFrame >= _animations[_animationIndex].frameCount) {
                _currentFrame = 0;
            }
            _lastUpdate = GetTime();
        }
    }
}
