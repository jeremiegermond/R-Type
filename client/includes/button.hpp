/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Button.hpp by thibb1
*/

#ifndef RTYPE_BUTTON_HPP
#define RTYPE_BUTTON_HPP

#include "deps.hpp"

class Button {
  private:
    std::string _text;
    Rectangle _bounds{};
    bool _pressed;
    bool _enabled;
    bool _hovered;
    Vector2 _textPosition{};
    Color _baseColor{};
    Color _selectedColor{};
    Color _hoveredColor{};
    Color _borderColor{};
    Color _selectedBorderColor{};
    std::function<void()> _callback;

  public:
    Button(const std::string &name, Rectangle bounds, bool enabled = true);
    ~Button() = default;
    bool UpdateButton();
    void Draw();
    void SetEnabled(bool enabled);
    template <typename T>
    void SetCallback(T &&callback) {
        _callback = std::forward<T>(callback);
    }
};

#endif // RTYPE_BUTTON_HPP
