/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
    /**
    * @brief Class used to handle CAnimation
    * 
    */
    class CAnimation : public ComponentBase {
      public:
        /**
         * @brief Construct a new CAnimation object
         */
        CAnimation();

        /**
         * @brief Destroy the CAnimation object
         */
        ~CAnimation() override;

        /**
         * @brief Unload the animations
         */
        void unloadAnimations();

        /**
         * @brief Get the pointer to the current animation
         * ////  return _animations.get()[_animationIndex];
         * @return  ModelAnimation
         */
        [[nodiscard]] ModelAnimation getAnimation() const;

        /**
         * @brief Set the animations
         * @param string animationPath path to the animations
         */
        void setAnimation(const std::string &animationPath);

        /**
         * @brief Set the animation index
         * @param unsigned int index
         */
        void setAnimationIndex(unsigned int index);

        /**
         * @brief Get the current frame
         * @return int
         */
        [[nodiscard]] int getCurrentFrame() const;

        /**
         * @brief Update the animation
         */
        void doUpdate() override;

      private:
        ModelAnimation *_animations;
        unsigned int _animationCount;
        unsigned int _animationIndex;
        int _currentFrame;
        double _lastUpdate;
    };
}
