/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** slider.cpp by thibb1
*/

#include "slider.hpp"

/**
 * It creates a new Slider object with the given name, bounds, value, minValue, maxValue, and enabled
 * 
 * @param name The name of the slider. This is used to label the slider.
 * @param bounds The rectangle that defines the size and position of the slider.
 * @param value The value that the slider will modify.
 * @param minValue The minimum value the slider can be set to.
 * @param maxValue The maximum value the slider can have.
 * @param enabled Whether or not the slider is enabled.
 */
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

/**
 * If the mouse is over the slider, and the left mouse button is pressed, the slider is dragged, and
 * the value is updated accordingly
 * 
 * @return A boolean value.
 */
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

/**
 * It draws a rectangle with a rectangle inside it
 * 
 * @return A pointer to the value of the slider.
 */
void Slider::Draw() {
    Color color = _baseColor;
    if (!_enabled)
        return;
    if (_dragging)
        color = _selectedColor;
    // draw slider base
    DrawRectangleRec(_bounds, color);
    // draw slider ball
    float x = (*_value - _minValue) / (_maxValue - _minValue) * (_bounds.width - 4) + _bounds.x;
    float y = _bounds.y + 2;
    int width = 4;
    int height = int(_bounds.height) - 4;
    DrawRectangle(int(x), int(y), width, height, BLACK);
    // draw name on the right
    DrawText(_name.c_str(), _bounds.x + _bounds.width + 10, _bounds.y + 4, 10, WHITE);
}
/**
 * Sets the enabled state of the slider.
 * 
 * @param enabled Whether or not the slider is enabled.
 */

void Slider::SetEnabled(bool enabled) { _enabled = enabled; }

/**
 * Set the value, minimum value, and maximum value of the slider.
 * 
 * @param value This is the value that the slider will be changing.
 * @param minValue The minimum value of the slider.
 * @param maxValue The maximum value of the slider.
 */
void Slider::SetValues(float *value, float minValue, float maxValue) {
    _value = value;
    _minValue = minValue;
    _maxValue = maxValue;
}

/**
 * It sets the value of the slider to the value of the float pointer passed in
 * 
 * @param value The value to set the slider to.
 * @param enable If true, the slider will be enabled.
 */
void Slider::SetValue(float *value, bool enable) {
    if (enable)
        _enabled = true;
    SetValues(value, _minValue, _maxValue);
}
