/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.cpp
*/

#include "engine/manager/ECSManagerBase.hpp"

namespace Engine {
    /**
     * ECSManagerBase::ECSManagerBase() {}
     */
    ECSManagerBase::ECSManagerBase() {}

    /**
     * ECSManagerBase::~ECSManagerBase() = default;
     */
    ECSManagerBase::~ECSManagerBase() = default;

    /**
     * It initializes the factories.
     */
    void ECSManagerBase::init() { initFactories(); }

    /**
     * `void ECSManagerBase::update() { updateGame(); }`
     * 
     * The above function is a function that is called every frame. It calls the `updateGame()`
     * function
     */
    void ECSManagerBase::update() { updateGame(); }
    
    /**
     * `void ECSManagerBase::destroy() { destroyGame(); }`
     * 
     * The function `destroy()` is a virtual function that is called when the game is destroyed. It
     * calls the function `destroyGame()` which is a virtual function that is called when the game is
     * destroyed
     */
    void ECSManagerBase::destroy() { destroyGame(); }
}
