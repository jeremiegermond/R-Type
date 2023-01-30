/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.cpp by thibb1
*/

#include "engine/components/ComponentBase.hpp"

namespace Engine {

ComponentBase::ComponentBase() : _initialized(false), _active(false), _messageManager(MessageManager::GetInstance()) {}

ComponentBase::~ComponentBase() { Destroy(); }

void ComponentBase::Init() {
    if (_initialized)
        return;
    RegisterMessageHandlers();
    DoInit();
    _initialized = true;
}

void ComponentBase::Update() {
    if (IsActive())
        DoUpdate();
}

void ComponentBase::Destroy() { DoDestroy(); }

void ComponentBase::SetActive(bool active) {
    if (_initialized)
        _active = active;
}

bool ComponentBase::IsActive() const { return _initialized && _active; }
} // namespace Engine
