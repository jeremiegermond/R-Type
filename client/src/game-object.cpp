/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game-object.cpp by thibb1
*/

#include "game-object.hpp"

GameObject::GameObject()
    : _position(Vector3Zero()), _velocity(Vector3Zero()), _rotation(Vector3Zero()), _rotationGoal(Vector3Zero()), _scale(1), _animations(nullptr),
      _animationCount(0), _currentAnimation(0), _currentFrame(0), _playAnimation(false), _loopAnimation(false) {
    _model = {};
    _boundingBox = getMyObjectBoundingBox();
}

GameObject::GameObject(const std::string &pathModel) : GameObject() { _model = LoadModel(pathModel.c_str()); }

GameObject::GameObject(GameObject *other) : GameObject() {
    _model = other->_model;
    _position = other->_position;
    _velocity = other->_velocity;
    _rotation = other->_rotation;
    _rotationGoal = other->_rotationGoal;
    _scale = other->_scale;
    _animations = other->_animations;
    _animationCount = other->_animationCount;
    _currentAnimation = other->_currentAnimation;
    _currentFrame = other->_currentFrame;
    _playAnimation = other->_playAnimation;
    _loopAnimation = other->_loopAnimation;
    _textures = other->_textures;
    _tags = other->_tags;
}

GameObject::~GameObject() {
    UnloadModel(_model);
    for (auto &texture : _textures) {
        UnloadTexture(*texture);
    }
    if (_animationCount > 0 && _animations != nullptr)
        UnloadModelAnimations(_animations, _animationCount);
    _textures.clear();
}

void GameObject::draw(Vector3 offset) { DrawModel(_model, Vector3Add(_position, offset), _scale, WHITE); }

void GameObject::update() {
    auto velocity = Vector3Scale(_velocity, GetFrameTime());
    _position = Vector3Add(_position, velocity);
    _rotation = Vector3Lerp(_rotation, _rotationGoal, 0.1f);
    _model.transform = MatrixRotateXYZ(_rotation);
    if (_animationCount > 0 && _playAnimation && _animations != nullptr) {
        _currentFrame++;
        if (_currentFrame >= _animations[_currentAnimation].frameCount) {
            if (_loopAnimation) {
                _currentFrame = 0;
            } else {
                _playAnimation = false;
                _currentFrame = _animations[_currentAnimation].frameCount - 1;
            }
        }
        UpdateModelAnimation(_model, _animations[_currentAnimation], _currentFrame);
    }
    _boundingBox = getMyObjectBoundingBox();
}

void GameObject::playAnimation(int index, bool loop) {
    if (index >= _animationCount)
        return;
    _currentAnimation = index;
    _currentFrame = 0;
    _playAnimation = true;
    _loopAnimation = loop;
}

void GameObject::addTag(const std::string &tag) {
    for (auto &t : _tags) {
        if (t == tag)
            return;
    }
    _tags.push_back(tag);
}

void GameObject::setPosition(Vector3 position) { _position = position; }

void GameObject::setVelocity(Vector3 velocity) { _velocity = velocity; }

void GameObject::setRotation(Vector3 rotation) {
    _rotation = rotation;
    _model.transform = MatrixRotateXYZ(_rotation);
}

void GameObject::setRotationGoal(Vector3 rotationGoal) { _rotationGoal = rotationGoal; }

void GameObject::setScale(float scale) { _scale = scale; }

void GameObject::setAnimations(const std::string &pathAnimations) {
    if (_animations != nullptr)
        UnloadModelAnimations(_animations, _animationCount);
    _animations = LoadModelAnimations(pathAnimations.c_str(), &_animationCount);
    if (_animationCount > 0)
        UpdateModelAnimation(_model, _animations[_currentAnimation], _currentFrame);
}

void GameObject::setShader(Shader shader) const {
    //_model.meshMaterial[0]
    for (int i = 0; i < _model.materialCount; i++)
        _model.materials[i].shader = shader;
}

void GameObject::setTexture(const std::string &pathTexture, MaterialMapIndex mapIndex, int index) {
    Texture2D texture = LoadTexture(pathTexture.c_str());
    _textures.push_back(&texture);
    if (index > 0 && index < _model.materialCount)
        setTextureMatIdx(texture, mapIndex, index);
    else
        for (int i = 0; i < _model.materialCount; i++)
            setTextureMatIdx(texture, mapIndex, i);
}

void GameObject::setTextureMatIdx(Texture2D texture, MaterialMapIndex mapIndex, int index) const {
    _model.materials[index].maps[mapIndex].texture = texture;
}

bool GameObject::isTagged(const std::string &tag) {
    if (std::ranges::any_of(_tags, [&tag](const std::string &t) { return t == tag; }))
        return true;
    return false;
}

BoundingBox GameObject::getBaseBoundingBox(Vector3 scale) {
    BoundingBox box = GetModelBoundingBox(_model);
    // scale box to match model size
    box.min = Vector3Scale(box.min, _scale);
    box.max = Vector3Scale(box.max, _scale);
    // Add scale to box
    box.min = Vector3Subtract(box.min, scale);
    box.max = Vector3Add(box.max, scale);
    return box;
}

BoundingBox GameObject::getMyObjectBoundingBox(Vector3 scale) {
    BoundingBox box = getBaseBoundingBox(scale);
    // translate box to match model position
    box.min = Vector3Add(box.min, _position);
    box.max = Vector3Add(box.max, _position);
    return box;
}
