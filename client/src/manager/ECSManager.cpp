/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManager.cpp
*/

#include "manager/ECSManager.hpp"

void ECSManager::initFactories() { addArchetype<ObjectArchetype>("Object"); }

void ECSManager::updateGame() {}

void ECSManager::destroyGame() {}
