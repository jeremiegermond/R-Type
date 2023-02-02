/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.cpp by thibb1
*/

#include "engine/manager/ECSManagerBase.hpp"

namespace Engine {
    ECSManagerBase::ECSManagerBase() : _testManager(nullptr) {}

    ECSManagerBase::~ECSManagerBase() = default;

    void ECSManagerBase::Init() {
        _testManager = new Archetype<float, int>();
        InitGame();
    }

    void ECSManagerBase::Update() { UpdateGame(); }

    void ECSManagerBase::Destroy() {
        DELETE_PTR(_testManager);
        DestroyGame();
    }
}
