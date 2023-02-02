/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** bullet.hpp by thibb1
*/

#ifndef RTYPE_BULLET_HPP
#define RTYPE_BULLET_HPP

#include "deps.hpp"

class Bullet {
    private:
        Vector3 _position;
        Vector3 _velocity;
        BoundingBox _boundingBox;
        float _scale;
        int _lightIndex;
        Color _color;
        // std::shared_ptr<Light> light;
    public:
        Bullet(Vector3 position, Vector3 velocity, float scale, int lightIndex, Color color);
        ~Bullet() = default;
        void Update();
        // draw using the given model
        void Draw(Model model);
        bool IsColliding(BoundingBox boundingBox);
        [[nodiscard]] Vector3 GetPosition() const;
        [[nodiscard]] int GetLightIndex() const;
        [[nodiscard]] float getSize() const;
        [[nodiscard]] Color getColor() const;
};

#endif // RTYPE_BULLET_HPP
