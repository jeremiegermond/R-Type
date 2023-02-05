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
    BoundingBox _boundingBox{};

  public:
    GameObject();
    explicit GameObject(const std::string &pathModel);
    explicit GameObject(GameObject *other);
    ~GameObject();

    void draw(Vector3 offset = Vector3Zero());
    void update();
    void playAnimation(int index, bool loop = false);

    void addTag(const std::string &tag);
    void setPosition(Vector3 position);
    void setVelocity(Vector3 velocity);
    void setRotation(Vector3 rotation);
    void setRotationGoal(Vector3 rotationGoal);
    void setScale(float scale);
    void setAnimations(const std::string &pathAnimations);
    void setShader(Shader shader) const;
    void setTexture(const std::string &pathTexture, MaterialMapIndex mapIndex, int index = -1);
    void setTextureMatIdx(Texture2D texture, MaterialMapIndex mapIndex, int index) const;
    bool isTagged(const std::string &tag);
    Vector3 getRotation() { return _rotation; }
    Vector3 getRotationGoal() { return _rotationGoal; }
    Vector3 getPosition() { return _position; }
    [[nodiscard]] float getScale() const { return _scale; }
    BoundingBox getBaseBoundingBox(Vector3 scale = Vector3Zero());
    BoundingBox getMyObjectBoundingBox(Vector3 scale = Vector3Zero());
    BoundingBox getBoundingBox() { return _boundingBox; }
};
#endif // RTYPE_GAME_OBJECT_HPP
