/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CHealth.cpp
*/

#include "components/CHealth.hpp"

CHealth::CHealth() : _health(0) {}

CHealth::~CHealth() = default;

void CHealth::setHealth(int health) { _health = health; }

int CHealth::getHealth() const { return _health; }

void CHealth::takeDamage(int damage) { _health -= damage; }
