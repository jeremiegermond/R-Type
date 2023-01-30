/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CTransform.cpp by thibb1
*/

#include "engine/components/CTransform.hpp"

namespace Engine {

CTransform::CTransform() : _localTransform(Transform{}), _worldTransform(Transform{}) {}

void CTransform::RegisterMessageHandlers() {
    RegisterMessageHandler(&CTransform::SetPosition);
    RegisterMessageHandler(&CTransform::GetPosition);
}

void CTransform::SetPosition(SetPositionMessage &message) {
    _localTransform.translation = message._position;
    UpdateWorldTransform();
}

void CTransform::GetPosition(GetPositionMessage &message) { message._position = _worldTransform.translation; }

void CTransform::UpdateWorldTransform() {}
} // namespace Engine
