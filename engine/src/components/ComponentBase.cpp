/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.cpp by thibb1
*/

#include "engine/components/ComponentBase.hpp"

namespace Engine {

    ComponentBase::ComponentBase() : _active(false) {}

    ComponentBase::~ComponentBase() { Destroy(); }

    void ComponentBase::Init() { DoInit(); }

    void ComponentBase::Update() {
        if (IsActive())
            DoUpdate();
    }

    void ComponentBase::Destroy() { DoDestroy(); }

    void ComponentBase::SetActive(bool active) { _active = active; }

    bool ComponentBase::IsActive() const { return _active; }
}
