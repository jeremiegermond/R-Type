/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** light.hpp by thibb1
*/

#ifndef RTYPE_LIGHT_HPP
#define RTYPE_LIGHT_HPP

#include <raylib.h>

enum LightType { LIGHT_DIRECTIONAL, LIGHT_POINT };
class Light {
  private:
    int _type;
    Vector3 _position;
    Vector3 _target;
    Color _color;
    float _intensity;
    int _enabled;
    int _enabledLoc;
    int _typeLoc;
    int _posLoc;
    int _targetLoc;
    int _colorLoc;
    int _intensityLoc;

  public:
    // Constructor
    Light(int i, LightType type, Vector3 position, Vector3 target, Color color, Shader shader, float intensity)
        : _type(type), _position(position), _target(target), _color(color), _intensity(intensity), _enabled(true) {
        _enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", i));
        _typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", i));
        _posLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", i));
        _targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", i));
        _colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", i));
        _intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", i));
        UpdateLightValues(shader);
    }

    // Destructor
    ~Light() = default;

    // Getters
    [[nodiscard]] Color getColor() const { return _color; }
    [[nodiscard]] Vector3 getPosition() const { return _position; }
    [[nodiscard]] bool isEnabled() const { return _enabled; }
    [[nodiscard]] float getIntensity() const { return _intensity; }

    // Setters
    void setPosition(Vector3 position) { _position = position; }
    void setTarget(Vector3 target) { _target = target; }
    void setEnabled(bool enabled) { _enabled = enabled; }
    void setColor(Color color) { _color = color; }
    void setIntensity(float intensity) { _intensity = intensity; }

    // Methods
    void UpdateLightValues(Shader shader) {
        SetShaderValue(shader, _enabledLoc, &_enabled, SHADER_UNIFORM_INT);
        SetShaderValue(shader, _typeLoc, &_type, SHADER_UNIFORM_INT);
        SetShaderValue(shader, _posLoc, &_position, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, _targetLoc, &_target, SHADER_UNIFORM_VEC3);
        float color[4] = {(float)_color.r / 255, (float)_color.g / 255, (float)_color.b / 255, (float)_color.a / 255};
        SetShaderValue(shader, _colorLoc, color, SHADER_UNIFORM_VEC4);
        SetShaderValue(shader, _intensityLoc, &_intensity, SHADER_UNIFORM_FLOAT);
    }
};
#endif // RTYPE_LIGHT_HPP
