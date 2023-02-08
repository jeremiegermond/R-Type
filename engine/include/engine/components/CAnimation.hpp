/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CAnimation.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {

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

  private:
    std::shared_ptr<ModelAnimation> _animations;
    unsigned int _animationCount;
    unsigned int _animationIndex;
};
} // namespace Engine
