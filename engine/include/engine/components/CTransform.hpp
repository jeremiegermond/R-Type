/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CTransform.hpp by thibb1
*/

#pragma once

#include "engine/components/ComponentBase.hpp"
#include "engine/messages/Messages.hpp"

namespace Engine {

class CTransform : public ComponentBase {
  public:
    CTransform();

    void RegisterMessageHandlers() override;

    void SetPosition(SetPositionMessage &message);
    void GetPosition(GetPositionMessage &message);

    void UpdateWorldTransform();

  private:
    Transform _localTransform;

    Transform _worldTransform;
};
} // namespace Engine
