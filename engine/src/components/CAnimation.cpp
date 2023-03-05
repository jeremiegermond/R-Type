/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.cpp
*/

#include "engine/components/CAnimation.hpp"

namespace Engine {
    /**
     * @brief Construct a new CAnimation::CAnimation object
     * 
     */
    CAnimation::CAnimation() : _animations(nullptr), _animationCount(0), _animationIndex(0), _currentFrame(0), _lastUpdate(GetTime()) {}

    /**
     * @brief The destructor for the CAnimation class
     */
    CAnimation::~CAnimation() { unloadAnimations(); }

    /**
     * @brief Unload the model animations from memory
     */
    void CAnimation::unloadAnimations() {
        if (_animations && _animationCount > 0) {
            UnloadModelAnimations(_animations, _animationCount);
            _animations = nullptr;
            _animationCount = 0;
            _animationIndex = 0;
        }
    }

    /**
     * If the animation is valid, return the animation
     * 
     * @return A copy of the animation at the current index.
     */
    ModelAnimation CAnimation::getAnimation() const {
        if (_animations && _animationCount > 0 && _animationIndex < _animationCount)
            return _animations[_animationIndex];
        return {};
    }

    /**
     * It loads the animation file and stores it in the _animations variable
     * 
     * @param animationPath The path to the animation file.
     */
    void CAnimation::setAnimation(const std::string &animationPath) {
        unloadAnimations();
        if (!FileExists(animationPath.c_str()))
            throw std::runtime_error("Animation file not found: " + animationPath);
        _animations = LoadModelAnimations(animationPath.c_str(), &_animationCount);
    }

    /**
     * If the index is less than the number of animations, set the animation index to the index and set
     * the current frame to 0
     * 
     * @param index The index of the animation to set.
     */
    void CAnimation::setAnimationIndex(unsigned int index) {
        if (index < _animationCount) {
            _animationIndex = index;
            _currentFrame = 0;
        }
    }

    /**
     * It returns the current frame of the animation.
     * 
     * @return The current frame of the animation.
     */
    int CAnimation::getCurrentFrame() const { return _currentFrame; }
    
    /**
     * If the current time minus the last update time is greater than 0.04, then increment the current
     * frame and if the current frame is greater than or equal to the frame count, then set the current
     * frame to 0
     */
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
