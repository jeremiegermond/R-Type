/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Defines.hpp by thibb1
*/

#pragma once

#define DELETE_PTR(x)                                                                                                                                \
    if (x) {                                                                                                                                         \
        delete x;                                                                                                                                    \
        x = nullptr;                                                                                                                                 \
    }
