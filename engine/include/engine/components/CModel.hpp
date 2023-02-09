/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CModel.hpp
*/

#pragma once

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

      private:
        Model _model;
        bool _loaded;
    };
}
