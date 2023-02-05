/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** game-object.cpp by thibb1
*/

#include "game-object.hpp"

/**
 * It initializes all the variables of the GameObject class to their default values
 */
GameObject::GameObject()
    : _position(Vector3Zero()), _velocity(Vector3Zero()), _rotation(Vector3Zero()), _rotationGoal(Vector3Zero()), _scale(1), _animations(nullptr),
      _animationCount(0), _currentAnimation(0), _currentFrame(0), _playAnimation(false), _loopAnimation(false) {
    _model = {};
    _boundingBox = getMyObjectBoundingBox();
}

/**
 * If the user doesn't specify a model, then use the default constructor, otherwise load the model from
 * the path specified.
 * 
 * @param pathModel The path to the model you want to load.
 */
GameObject::GameObject(const std::string &pathModel) : GameObject() { _model = LoadModel(pathModel.c_str()); }

/**
 * It copies all the data from the other GameObject into this one
 * 
 * @param other The GameObject to copy.
 */
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

/**
 * Unload the model, unload all the textures, unload the animations and clear the texture vector
 */
GameObject::~GameObject() {
    UnloadModel(_model);
    for (auto &texture : _textures) {
        UnloadTexture(*texture);
    }
    if (_animationCount > 0 && _animations != nullptr)
        UnloadModelAnimations(_animations, _animationCount);
    _textures.clear();
}
/**
 * Draws the model at the position of the object, with the scale of the object, and in white
 * 
 * @param offset This is the offset of the model. This is used to draw the model at a different
 * position than the position of the object.
 */

void GameObject::draw(Vector3 offset) { DrawModel(_model, Vector3Add(_position, offset), _scale, WHITE); }

/**
 * It updates the position, rotation, and animation of the object
 */
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

/**
 * It sets the current animation to the one specified by the index, and sets the current frame to 0. It
 * also sets the _playAnimation flag to true, and the _loopAnimation flag to the value of the loop
 * parameter
 * 
 * @param index The index of the animation to play.
 * @param loop If true, the animation will loop. If false, the animation will play once and then stop.
 * 
 * @return The current animation index.
 */
void GameObject::playAnimation(int index, bool loop) {
    if (index >= _animationCount)
        return;
    _currentAnimation = index;
    _currentFrame = 0;
    _playAnimation = true;
    _loopAnimation = loop;
}

/**
 * If the tag is not already in the list of tags, add it
 * 
 * @param tag The tag to add to the GameObject.
 * 
 * @return A reference to the GameObject's tag vector.
 */
void GameObject::addTag(const std::string &tag) {
    for (auto &t : _tags) {
        if (t == tag)
            return;
    }
    _tags.push_back(tag);
}
/**
 * Sets the position of the GameObject to the given position.
 * 
 * @param position The position of the object in the world.
 */

void GameObject::setPosition(Vector3 position) { _position = position; }

/**
 * Sets the velocity of the GameObject to the given Vector3.
 * 
 * @param velocity The velocity of the object.
 */
void GameObject::setVelocity(Vector3 velocity) { _velocity = velocity; }
/**
 * It sets the rotation of the object to the given rotation
 * 
 * @param rotation The rotation of the object in degrees.
 */

void GameObject::setRotation(Vector3 rotation) {
    _rotation = rotation;
    _model.transform = MatrixRotateXYZ(_rotation);
}
/**
 * It sets the rotation goal of the GameObject to the given Vector3.
 * 
 * @param rotationGoal The goal rotation of the object.
 */

void GameObject::setRotationGoal(Vector3 rotationGoal) { _rotationGoal = rotationGoal; }
/**
 * Sets the scale of the GameObject.
 * 
 * @param scale The scale of the object.
 */

void GameObject::setScale(float scale) { _scale = scale; }

/**
 * It loads the model animations from a file and sets the current animation and frame
 * 
 * @param pathAnimations Path to the animations file.
 */
void GameObject::setAnimations(const std::string &pathAnimations) {
    if (_animations != nullptr)
        UnloadModelAnimations(_animations, _animationCount);
    _animations = LoadModelAnimations(pathAnimations.c_str(), &_animationCount);
    if (_animationCount > 0)
        UpdateModelAnimation(_model, _animations[_currentAnimation], _currentFrame);
}

/**
 * It sets the shader of the model's materials to the shader passed in
 * 
 * @param shader The shader to set the material to.
 */
void GameObject::setShader(Shader shader) const {
    //_model.meshMaterial[0]
    for (int i = 0; i < _model.materialCount; i++)
        _model.materials[i].shader = shader;
}

/**
 * It loads a texture from a file, stores it in a vector, and then sets the texture for the model's
 * material
 * 
 * @param pathTexture The path to the texture file.
 * @param mapIndex The type of map to set.
 * @param index the index of the material to set the texture to. If you don't know the index, you can
 * set it to -1 and it will set the texture to all materials.
 */
void GameObject::setTexture(const std::string &pathTexture, MaterialMapIndex mapIndex, int index) {
    Texture2D texture = LoadTexture(pathTexture.c_str());
    _textures.push_back(&texture);
    if (index > 0 && index < _model.materialCount)
        setTextureMatIdx(texture, mapIndex, index);
    else
        for (int i = 0; i < _model.materialCount; i++)
            setTextureMatIdx(texture, mapIndex, i);
}

/**
 * Sets the texture of the specified material map index of the specified material index of the model of
 * this game object.
 * 
 * @param texture The texture to set.
 * @param mapIndex The index of the map in the material.
 * @param index The index of the material in the model's material array.
 */
void GameObject::setTextureMatIdx(Texture2D texture, MaterialMapIndex mapIndex, int index) const {
    _model.materials[index].maps[mapIndex].texture = texture;
}

/**
 * If any of the tags in the vector of tags is equal to the tag passed in, return true
 * 
 * @param tag The tag to check for.
 * 
 * @return A boolean value.
 */
bool GameObject::isTagged(const std::string &tag) {
    if (std::ranges::any_of(_tags, [&tag](const std::string &t) { return t == tag; }))
        return true;
    return false;
}

/**
 * It returns a bounding box that is the same size as the model, but with the given scale added to it
 * 
 * @param scale The scale of the bounding box.
 * 
 * @return A BoundingBox object.
 */
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

/**
 * It returns a bounding box that is the same as the base bounding box, but translated to the model's
 * position
 * 
 * @param scale The scale of the object.
 * 
 * @return A BoundingBox object.
 */
BoundingBox GameObject::getMyObjectBoundingBox(Vector3 scale) {
    BoundingBox box = getBaseBoundingBox(scale);
    // translate box to match model position
    box.min = Vector3Add(box.min, _position);
    box.max = Vector3Add(box.max, _position);
    return box;
}
