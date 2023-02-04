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
    Rectangle _bounds{};
    float *_value;
    float _minValue;
    float _maxValue;
    bool _dragging;
    bool _enabled;
    Color _baseColor{};
    Color _selectedColor{};
    std::function<void(float *)> _callback;

  public:
    Slider(const std::string &name, Rectangle bounds, float *value, float minValue, float maxValue, bool enabled = true);
    bool UpdateSlider();
    void Draw();
    void SetEnabled(bool enabled);
    void SetValues(float *value, float minValue, float maxValue);
    void SetValue(float *value, bool enable = true);

    // callback should have access to _value
    template <typename T>
    void SetCallback(T &&callback) {
        _callback = [this, callback](float *value) { callback(_value); };
    }
};

#endif // RTYPE_SLIDER_HPP
