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
        if (_loaded)
            UnloadModel(_model);
        _loaded = false;
    }

    Model CModel::getModel() const { return _model; }

    void CModel::setModel(const std::string &modelPath) {
        unloadModel();
        if (!FileExists(modelPath.c_str()))
            throw std::runtime_error("Model file not found: " + modelPath);
        //_model = std::make_shared<Model>(LoadModel(modelPath.c_str()));
        _model = LoadModel(modelPath.c_str());
        _loaded = true;
    }
}
