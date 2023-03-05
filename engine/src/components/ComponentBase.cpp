/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ComponentBase.cpp
*/

#include "engine/components/ComponentBase.hpp"

namespace Engine {

    /**
     * ComponentBase::ComponentBase() : _active(false) {}
     */
    ComponentBase::ComponentBase() : _active(false) {}

    /**
     * It destroys the component.
     */
    ComponentBase::~ComponentBase() { destroy(); }

    /**
     * The function `doInit()` is called from the function `init()` which is called from the
     * constructor
     */
    void ComponentBase::init() { doInit(); }

    /**
     * If the component is active, call the doUpdate() function
     */
    void ComponentBase::update() {
        if (isActive())
            doUpdate();
    }

    /**
     * If the object is not already destroyed, then destroy it.
     */
    void ComponentBase::destroy() { doDestroy(); }

    /**
     * It sets the active state of the component.
     * 
     * @param active Whether or not the component is active.
     */
    void ComponentBase::setActive(bool active) { _active = active; }

    /**
     * `ComponentBase::isActive()` returns `_active`
     * 
     * @return A reference to the component's parent entity.
     */
    bool ComponentBase::isActive() const { return _active; }
}
