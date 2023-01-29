/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** particle2d.hpp by thibb1
*/

#ifndef RTYPE_PARTICLE2D_HPP
#define RTYPE_PARTICLE2D_HPP

#include "deps.hpp"
#include "structs.hpp"

class Particle2D {
  private:
    MyTexture *_texture;
    Vector3 _position;
    Vector3 _up;
    Vector2 _scale;
    Rectangle _source{};
    int _totalFrames;
    int _currentFrame;
    int _columns;
    int _rows;
    int _width;
    int _height;
    float _rotation;
    bool _enabled;

  public:
    Particle2D(MyTexture *texture, Vector3 position, Vector2 scale = Vector2Zero(), float rotation = 0);
    ~Particle2D() = default;
    void Update();
    void Draw(Camera3D camera);
    [[nodiscard]] bool IsEnabled() const;
};

#endif // RTYPE_PARTICLE2D_HPP
