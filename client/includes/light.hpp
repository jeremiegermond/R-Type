/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** light.hpp by thibb1
*/

#ifndef RTYPE_LIGHT_HPP
#define RTYPE_LIGHT_HPP

#include "deps.hpp"

enum LightType { LIGHT_DIRECTIONAL, LIGHT_POINT };
class Light {
  private:
    int _index;
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
    Light(int index, LightType type, Vector3 position, Vector3 target, Color color, Shader shader, float intensity)
        : _index(index), _type(type), _position(position), _target(target), _color(color), _intensity(intensity), _enabled(true) {
        _enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", _index));
        _typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", _index));
        _posLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", _index));
        _targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", _index));
        _colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", _index));
        _intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", _index));
        UpdateLightValues(shader);
    }

    // Destructor
    ~Light() = default;

    // Getters
    [[nodiscard]] Color getColor() const { return _color; }
    [[nodiscard]] Vector3 getPosition() const { return _position; }
    [[nodiscard]] bool isEnabled() const { return _enabled; }
    [[nodiscard]] float getIntensity() const { return _intensity; }
    [[nodiscard]] int getIndex() const { return _index; }

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

    void DrawSphere() { DrawSphereWires(_position, .2, 4, 4, _color); }
};
#endif // RTYPE_LIGHT_HPP
