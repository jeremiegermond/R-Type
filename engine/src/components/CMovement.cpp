/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CMovement.cpp by thibb1
*/

#include <utility>

#include "engine/components/CMovement.hpp"

namespace Engine {

    CMovement::CMovement() : _speed(Vector3{0, 0, 0}), _position(nullptr) {}

    void CMovement::DoUpdate() {
        if (!_position)
            return;
        Vector3 speed = GetSpeed();
        _position->AddPosition(speed);
    }

    void CMovement::SetSpeed(Vector3 speed) { _speed = speed; }

    Vector3 CMovement::GetSpeed() const {
        float delta = GetFrameTime();
        Vector3 speed = Vector3Multiply(_speed, Vector3{delta, delta, delta});
        return speed;
    }

    void CMovement::SetPosition(std::shared_ptr<CPosition> position) {
        std::cout << "CMovement::SetPosition" << std::endl;
        _position = std::move(position);
        // out the pointer
        std::cout << "CMovement::SetPosition: " << _position << std::endl;
    }
}
