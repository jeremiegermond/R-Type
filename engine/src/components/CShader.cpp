/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CShader.cpp
*/

#include "engine/components/CShader.hpp"

namespace Engine {
CShader::CShader() : _shader(nullptr) {}

CShader::~CShader() {
    std::cout << "CShader destructor" << std::endl;
    if (_shader != nullptr) {
        UnloadShader(*_shader);
    }
}

std::shared_ptr<Shader> CShader::getShader() const { return _shader; }

void CShader::setShader(const std::string &fragmentPath, const std::string &vertexPath) {
    if (_shader != nullptr) {
        UnloadShader(*_shader);
    }
    _shader = std::make_shared<Shader>();
    *_shader = LoadShader(vertexPath.c_str(), fragmentPath.c_str());
}

void CShader::setShaderLocation(const std::string &locationName, const std::string &shaderFrom) {
    if (locationName == "loc_matrix_model") {
        _shader->locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(*_shader, shaderFrom.c_str());
    } else if (locationName == "loc_vector_view") {
        _shader->locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(*_shader, shaderFrom.c_str());
    }
}

} // namespace Engine
