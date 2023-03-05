/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CModel.cpp
*/

#include "engine/components/CModel.hpp"

namespace Engine {
    /**
     * The constructor for the CModel class.
     */
    CModel::CModel() : _model(), _loaded(false) {}

    /**
     * The destructor for the CModel class.
     */
    CModel::~CModel() { unloadModel(); }

   
    /**
     * UnloadModel() is a function that takes a model as a parameter and unloads it
     */
    void CModel::unloadModel() {
        if (_loaded) {
            UnloadModel(*_model);
            _textures.clear();
        }
        _loaded = false;
    }

    /**
     * It returns the model.
     * 
     * @return A pointer to the model.
     */
    Model CModel::getModel() const { return *_model; }

    /**
     * "Loads a model from a file and stores it in the _model variable."
     * 
     * The first thing we do is unload the model if it's already loaded. This is done by calling the
     * unloadModel() function
     * 
     * @param modelPath The path to the model file.
     */
    void CModel::setModel(const std::string &modelPath) {
        unloadModel();
        if (!FileExists(modelPath.c_str()))
            throw std::runtime_error("Model file not found: " + modelPath);
        // LoadModel(modelPath.c_str()); returns Model
        _model = new Model(LoadModel(modelPath.c_str()));
        _loaded = true;
    }

    /**
     * It takes a texture name and a texture path, checks if the model is loaded and the texture file
     * exists, then adds the texture to the model
     * 
     * @param textureName The name of the texture. This can be "diffuse", "normal", or "metallic".
     * @param texturePath The path to the texture file.
     */
    void CModel::setModelTexture(const std::string &textureName, const std::string &texturePath) {
        if (!_loaded)
            throw std::runtime_error("Model not loaded");
        if (!FileExists(texturePath.c_str()))
            throw std::runtime_error("Texture file not found: " + texturePath);
        _textures.emplace_back(texturePath.c_str());
        if (textureName == "diffuse") {
            _model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *_textures.back().getTexture();
        } else if (textureName == "normal") {
            _model->materials[0].maps[MATERIAL_MAP_NORMAL].texture = *_textures.back().getTexture();
        } else if (textureName == "metallic") {
            _model->materials[0].maps[MATERIAL_MAP_METALNESS].texture = *_textures.back().getTexture();
        } else {
            throw std::runtime_error("Texture name not supported: " + textureName);
        }
    }

    /**
     * It sets the shader for each material in the model
     * 
     * @param shader The shader to set the model to use.
     */
    void CModel::setModelShader(const std::shared_ptr<Shader> &shader) const {
        if (!_loaded)
            throw std::runtime_error("Model not loaded");
        for (int i = 0; i < _model->materialCount; i++) {
            _model->materials[i].shader = *shader;
        }
    }
}
