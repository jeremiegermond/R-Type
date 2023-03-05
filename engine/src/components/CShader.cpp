/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CShader.cpp
*/

#include "engine/components/CShader.hpp"

namespace Engine {
    CShader::CShader() : _shader(nullptr) {}

    /**
     * If the shader is not null, unload the shader
     */
    CShader::~CShader() {
        std::cout << "CShader destructor" << std::endl;
        if (_shader != nullptr) {
            UnloadShader(*_shader);
        }
    }

    /**
     * It returns a pointer to the shader object.
     * 
     * @return A pointer to a shared_ptr of a Shader object.
     */
    std::shared_ptr<Shader> CShader::getShader() const { return _shader; }

    /**
     * It takes a fragment and vertex shader path, and loads the shader
     * 
     * @param fragmentPath The path to the fragment shader
     * @param vertexPath The path to the vertex shader file.
     */
    void CShader::setShader(const std::string &fragmentPath, const std::string &vertexPath) {
        if (_shader != nullptr) {
            UnloadShader(*_shader);
        }
        _shader = std::make_shared<Shader>();
        *_shader = LoadShader(vertexPath.c_str(), fragmentPath.c_str());
    }

    /**
     * It takes a string, and if it matches a string in the array, it sets the location of the shader
     * to the location of the string in the array
     * 
     * @param locationName The name of the location in the shader.
     * @param shaderFrom The name of the variable in the shader.
     */
    void CShader::setShaderLocation(const std::string &locationName, const std::string &shaderFrom) {
        if (locationName == "loc_matrix_model") {
            _shader->locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(*_shader, shaderFrom.c_str());
        } else if (locationName == "loc_vector_view") {
            _shader->locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(*_shader, shaderFrom.c_str());
        }
    }

}
