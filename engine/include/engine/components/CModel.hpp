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
         * @brief Get the pointer to the model
         * @return std::shared_ptr<Model>
         */
        [[nodiscard]] std::shared_ptr<Model> getModel() const;

        /**
         * @brief Set the model
         * @param string modelPath path to the model
         */
        void setModel(const std::string &modelPath);

      private:
        std::shared_ptr<Model> _model;
    };
}
