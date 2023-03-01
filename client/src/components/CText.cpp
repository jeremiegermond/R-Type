/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CText.cpp
*/

#include "components/CText.hpp"

CText::CText() = default;

const std::string &CText::getText() const { return _text; }

void CText::setText(const std::string &text) { _text = text; }

int CText::getFontSize() const { return _fontSize; }

void CText::setFontSize(int fontSize) { _fontSize = fontSize; }
