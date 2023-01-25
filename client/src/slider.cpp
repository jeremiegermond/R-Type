/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** slider.cpp by thibb1
*/

#include "rtype-client.hpp"

Slider CreateSlider(Rectangle bounds, float value, float minValue, float maxValue) {
    Slider slider = {0};
    slider.bounds = bounds;
    slider.value = value;
    slider.minValue = minValue;
    slider.maxValue = maxValue;
    slider.dragging = false;
    slider.enabled = true;
    slider.baseColor = Fade(LIGHTGRAY, .6);
    slider.selectedColor = Fade(LIGHTGRAY, .9);
    slider.disabledColor = Fade(LIGHTGRAY, .3);
    return slider;
}

void UpdateSlider(Slider *slider) {
    if (!slider->enabled)
        return;
    if (CheckCollisionPointRec(GetMousePosition(), slider->bounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            slider->dragging = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        slider->dragging = false;
    if (slider->dragging) {
        slider->value = (GetMouseX() - slider->bounds.x) / slider->bounds.width;
        slider->value = Clamp(slider->value, 0, 1);
    }
}

void DrawSlider(Slider slider) {
    Color color = slider.baseColor;
    if (!slider.enabled)
        color = slider.disabledColor;
    else if (slider.dragging)
        color = slider.selectedColor;
    // Draw slider base
    DrawRectangleRec(slider.bounds, color);
    // Draw slider ball
    int x = slider.bounds.x + (slider.bounds.width - 4) * slider.value;
    int y = slider.bounds.y + 2;
    int width = 4;
    int height = slider.bounds.height - 4;
    DrawRectangle(x, y, width, height, BLACK);
}
