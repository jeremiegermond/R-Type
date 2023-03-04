/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CHealth.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CHealth : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CHealth object
     */
    CHealth();

    /**
     * @brief Destroy the CHealth object
     */
    ~CHealth() override;

    /**
     * @brief Set the health
     * @param int health
     */
    void setHealth(int health);

    /**
     * @brief Get the health
     * @return int
     */
    [[nodiscard]] int getHealth() const;

    /**
     * @brief Take damage
     * @param int damage
     */
    void takeDamage(int damage);

  private:
    int _health;
};
