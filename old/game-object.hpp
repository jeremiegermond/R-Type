/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game-object.hpp by thibb1
*/

#ifndef RTYPE_GAME_OBJECT_HPP
#define RTYPE_GAME_OBJECT_HPP
#include "deps.hpp"
class GameObject {
  private:
    std::vector<std::string> _tags;
    Vector3 _position;
    Vector3 _velocity;
    Vector3 _rotation;
    Vector3 _rotationGoal;
    float _scale;
    ModelAnimation *_animations;
    unsigned int _animationCount;
    int _currentAnimation;
    int _currentFrame;
    bool _playAnimation;
    bool _loopAnimation;
    Model _model{};
    std::vector<Texture2D *> _textures;

  public:
    explicit GameObject(const std::string &pathModel);
    ~GameObject();

    void Draw(Vector3 offset = Vector3Zero());
    void Update();
    void PlayAnimation(int index, bool loop = false);

    void SetTag(const std::string &tag);
    void SetPosition(Vector3 position);
    void SetVelocity(Vector3 velocity);
    void SetRotation(Vector3 rotation);
    void SetRotationGoal(Vector3 rotationGoal);
    void SetScale(float scale);
    void SetAnimations(const std::string &pathAnimations);
    void SetShader(Shader shader) const;
    void SetTexture(const std::string &pathTexture, MaterialMapIndex mapIndex, int index = -1);
    void SetTextureMatIdx(Texture2D texture, MaterialMapIndex mapIndex, int index) const;
    bool IsTagged(const std::string &tag);
    Vector3 GetRotation() { return _rotation; }
    Vector3 GetRotationGoal() { return _rotationGoal; }
    Vector3 GetPosition() { return _position; }
    [[nodiscard]] float GetScale() const { return _scale; }
    BoundingBox GetMyObjectBoundingBox(Vector3 scale = Vector3Zero());
};
#endif // RTYPE_GAME_OBJECT_HPP
