/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.cpp
*/

#include "engine/manager/ECSManagerBase.hpp"

namespace Engine {
ECSManagerBase::ECSManagerBase() {}

ECSManagerBase::~ECSManagerBase() = default;

void ECSManagerBase::init() { initFactories(); }

void ECSManagerBase::update() { updateGame(); }

void ECSManagerBase::destroy() { destroyGame(); }
} // namespace Engine
