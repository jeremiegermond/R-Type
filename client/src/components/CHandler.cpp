/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CHandler.cpp
*/

#include "components/CHandler.hpp"

CHandler::CHandler() {
    _handler = []() {};
}

void CHandler::execute() { _handler(); }

void CHandler::setHandler(std::function<void()> handler) { _handler = std::move(handler); }
