/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** slider.hpp by thibb1
*/

#ifndef RTYPE_SLIDER_HPP
#define RTYPE_SLIDER_HPP

#include "deps.hpp"

class Slider {
  private:
    std::string _name;
    Rectangle _bounds;
    float *_value;
    float _minValue;
    float _maxValue;
    bool _dragging;
    bool _enabled;
    Color _baseColor;
    Color _selectedColor;
    Color _disabledColor;

  public:
    Slider(std::string name, Rectangle bounds, float *value, float minValue, float maxValue);
    bool UpdateSlider();
    void Draw();
    void SetEnabled(bool enabled);
};

#endif // RTYPE_SLIDER_HPP
