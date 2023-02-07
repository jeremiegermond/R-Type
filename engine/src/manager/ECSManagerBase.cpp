/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.cpp by thibb1
*/

#include "engine/manager/ECSManagerBase.hpp"

namespace Engine {
    ECSManagerBase::ECSManagerBase() {}

    ECSManagerBase::~ECSManagerBase() = default;

    void ECSManagerBase::init() { initGame(); }

    void ECSManagerBase::update() { updateGame(); }

    void ECSManagerBase::destroy() { destroyGame(); }
}
