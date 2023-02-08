/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CModel.cpp
*/

#include "engine/components/CModel.hpp"

namespace Engine {
//    private:
// std::shared_ptr<Model> _model;

CModel::CModel() : _model(nullptr) {}

CModel::~CModel() {
    if (_model)
        UnloadModel(*_model);
}

std::shared_ptr<Model> CModel::getModel() const { return _model; }

void CModel::setModel(const std::string &modelPath) {
    if (_model)
        UnloadModel(*_model);
    _model = std::make_shared<Model>(LoadModel(modelPath.c_str()));
}
} // namespace Engine
