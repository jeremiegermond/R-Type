/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CVelocity.hpp
*/

#pragma once

#include "engine/components/CPosition.hpp"
#include "engine/components/ComponentBase.hpp"

namespace Engine {

class CVelocity : public ComponentBase {
  public:
    CVelocity();
    ~CVelocity() override = default;

    void doUpdate() override;

    void setSpeed(Vector3 speed);
    [[nodiscard]] Vector3 getSpeed() const;

    void setPosition(std::shared_ptr<CPosition> position);

  private:
    Vector3 _speed;
    Vector3 _frameSpeed;
    std::shared_ptr<CPosition> _position;
};
} // namespace Engine
