/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** slider.cpp by thibb1
*/

#include "slider.hpp"

Slider::Slider(const std::string &name, Rectangle bounds, float *value, float minValue, float maxValue, bool enabled) {
    _name = name;
    _bounds = bounds;
    _value = value;
    _minValue = minValue;
    _maxValue = maxValue;
    _dragging = false;
    _enabled = enabled;
    _baseColor = Fade(LIGHTGRAY, .6);
    _selectedColor = Fade(LIGHTGRAY, .9);
}

bool Slider::UpdateSlider() {
    if (!_enabled)
        return false;
    if (CheckCollisionPointRec(GetMousePosition(), _bounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            _dragging = true;
        // check mouse over scroll up / down
        float mouseWheelMove = GetMouseWheelMove();
        if (mouseWheelMove != 0) {
            *_value += mouseWheelMove * (_maxValue - _minValue) / 100;
            *_value = Clamp(*_value, _minValue, _maxValue);
            if (_callback)
                _callback(_value);
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        _dragging = false;
        if (_callback)
            _callback(_value);
    }
    if (_dragging) {
        // set value between _minValue and _maxValue
        float x = GetMousePosition().x - _bounds.x;
        x = Clamp(x, 0, _bounds.width - 4);
        *_value = x / (_bounds.width - 4) * (_maxValue - _minValue) + _minValue;
    }
    return _dragging;
}

void Slider::Draw() {
    Color color = _baseColor;
    if (!_enabled)
        return;
    if (_dragging)
        color = _selectedColor;
    // Draw slider base
    DrawRectangleRec(_bounds, color);
    // Draw slider ball
    float x = (*_value - _minValue) / (_maxValue - _minValue) * (_bounds.width - 4) + _bounds.x;
    float y = _bounds.y + 2;
    int width = 4;
    int height = int(_bounds.height) - 4;
    DrawRectangle(int(x), int(y), width, height, BLACK);
    // draw name on the right
    DrawText(_name.c_str(), _bounds.x + _bounds.width + 10, _bounds.y + 4, 10, WHITE);
}

void Slider::SetEnabled(bool enabled) { _enabled = enabled; }
