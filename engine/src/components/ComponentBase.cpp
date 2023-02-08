/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.cpp
*/

#include "engine/components/ComponentBase.hpp"

namespace Engine {

    ComponentBase::ComponentBase() : _active(false) {}

    ComponentBase::~ComponentBase() { destroy(); }

    void ComponentBase::init() { doInit(); }

    void ComponentBase::update() {
        if (isActive())
            doUpdate();
    }

    void ComponentBase::destroy() { doDestroy(); }

    void ComponentBase::setActive(bool active) { _active = active; }

    bool ComponentBase::isActive() const { return _active; }
}
