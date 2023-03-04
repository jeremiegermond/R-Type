/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CSound.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

namespace Engine {

    class CSound : public ComponentBase {
      public:
        /**
         * @brief Construct a new CSound object
         */
        CSound();

        /**
         * @brief Destroy the CSound object
         */
        ~CSound() override;

        /**
         * @brief Unload the sound
         */
        void unload();

        /**
         * @brief Get the sound
         * @return Sound sound
         */
        [[nodiscard]] Sound getSound() const;

        /**
         * @brief Set the sound
         * @param string soundPath path to the sound
         */
        void setSound(const std::string &soundPath);

        /**
         * @brief Set the sound volume
         * @param float volume
         */
        void setVolume(float volume);

        /**
         * @brief Get the sound volume
         * @return float
         */
        [[nodiscard]] float getVolume() const;

      private:
        Sound _sound;
        float _volume;
        bool _loaded;
    };
}
