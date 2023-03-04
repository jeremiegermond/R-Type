/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CModel.cpp
*/

#include "engine/components/CModel.hpp"

namespace Engine {
    CModel::CModel() : _model(), _loaded(false) {}

    CModel::~CModel() { unloadModel(); }

    void CModel::unloadModel() {
        if (_loaded) {
            UnloadModel(*_model);
            _textures.clear();
        }
        _loaded = false;
    }

    Model CModel::getModel() const { return *_model; }

    void CModel::setModel(const std::string &modelPath) {
        unloadModel();
        if (!FileExists(modelPath.c_str()))
            throw std::runtime_error("Model file not found: " + modelPath);
        // LoadModel(modelPath.c_str()); returns Model
        _model = new Model(LoadModel(modelPath.c_str()));
        _loaded = true;
    }

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

    void CModel::setModelShader(const std::shared_ptr<Shader> &shader) const {
        if (!_loaded)
            throw std::runtime_error("Model not loaded");
        for (int i = 0; i < _model->materialCount; i++) {
            _model->materials[i].shader = *shader;
        }
    }
}
