/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CMovement.hpp by thibb1
*/

#pragma once

#include "engine/components/CPosition.hpp"
#include "engine/components/ComponentBase.hpp"

namespace Engine {

    class CMovement : public ComponentBase {
        public:
            CMovement();
            ~CMovement() override = default;

            void DoUpdate() override;

            void SetSpeed(Vector3 speed);
            [[nodiscard]] Vector3 GetSpeed() const;

            void SetPosition(std::shared_ptr<CPosition> position);

        private:
            Vector3 _speed;
            std::shared_ptr<CPosition> _position;
    };
}
