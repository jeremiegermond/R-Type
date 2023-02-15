/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CModel.hpp
*/

#pragma once

#include "engine/components/CTexture.hpp"
#include "engine/components/ComponentBase.hpp"

namespace Engine {

    class CModel : public ComponentBase {
      public:
        /**
         * @brief Construct a new CModel object
         */
        CModel();

        /**
         * @brief Destroy the CModel object
         */
        ~CModel() override;

        /**
         * @brief Unload the model
         */
        void unloadModel();

        /**
         * @brief Get the model
         * @return Model
         */
        [[nodiscard]] Model getModel() const;

        /**
         * @brief Set the model
         * @param string modelPath path to the model
         */
        void setModel(const std::string &modelPath);

        /**
         * @brief Set model texture
         * @param string textureName path to the texture
         * @param string texturePath name of the texture
         */
        void setModelTexture(const std::string &textureName, const std::string &texturePath);

        /**
         * @brief Set model shader
         * @param std::shared_ptr<Shader> shader
         */
        void setModelShader(const std::shared_ptr<Shader> &shader) const;

      private:
        Model _model;
        std::vector<Engine::CTexture> _textures;
        bool _loaded;
    };
}
