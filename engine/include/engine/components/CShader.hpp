/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CShader.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
class CShader : public ComponentBase {
  public:
    /**
     * @brief Construct a new CShader object
     */
    CShader();

    /**
     * @brief Destroy the CShader object
     */
    ~CShader() override;

    /**
     * @brief Get the pointer to the shader
     * @return std::shared_ptr<Shader>
     */
    [[nodiscard]] std::shared_ptr<Shader> getShader() const;

    /**
     * @brief Set the shader
     * @param string fragmentPath path to the fragment shader
     * @param string vertexPath path to the vertex shader
     */
    void setShader(const std::string &fragmentPath, const std::string &vertexPath);

    /**
     * @brief Set shader location
     * @param string locationName name of the location
     * @param string shaderFrom shader to get the location from
     */
    void setShaderLocation(const std::string &locationName, const std::string &shaderFrom);

  private:
    std::shared_ptr<Shader> _shader;
};
} // namespace Engine
