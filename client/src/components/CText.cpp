/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CText.cpp
*/

#include "components/CText.hpp"

CText::CText() : _text(), _fontSize(20), _offset(Vector3Zero()) {}

const std::string &CText::getText() const { return _text; }

void CText::setText(const std::string &text) { _text = text; }

float CText::getFontSize() const { return _fontSize; }

void CText::setFontSize(float fontSize) { _fontSize = fontSize; }

void CText::setOffset(Vector3 offset) { _offset = offset; }

Vector3 CText::getOffset() { return _offset; }
