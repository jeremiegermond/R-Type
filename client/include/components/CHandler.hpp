/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CHandler.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CHandler : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CHandler object
     */
    CHandler();

    /**
     * @brief Destroy the CHandler object
     */
    ~CHandler() override = default;

    /**
     * @brief Execute the handler
     */
    void execute();

    /**
     * @brief Set the handler
     * @param handler The handler to set
     */
    void setHandler(std::function<void()> handler);

  private:
    std::function<void()> _handler;
};
