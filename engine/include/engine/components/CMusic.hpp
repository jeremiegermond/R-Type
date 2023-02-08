/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CMusic.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {
class CMusic : public ComponentBase {
  public:
    /**
     * @brief Construct a new CMusic object
     */
    CMusic();

    /**
     * @brief Destroy the CMusic object
     */
    ~CMusic() override;

    /**
     * @brief Get the pointer to the music
     * @return std::shared_ptr<Music>
     */
    [[nodiscard]] std::shared_ptr<Music> getMusic() const;

    /**
     * @brief Set the music
     * @param string musicPath path to the music
     */
    void setMusic(const std::string &musicPath);

    /**
     * @brief Set if the music should loop
     * @param bool loop
     */
    void setLoop(bool loop);

  private:
    std::shared_ptr<Music> _music;
};
} // namespace Engine
