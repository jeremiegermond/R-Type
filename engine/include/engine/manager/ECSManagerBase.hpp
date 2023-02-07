/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ECSManagerBase.hpp by thibb1
*/

#pragma once

#include "engine/graphics/Raylib.hpp"
#include "engine/manager/Archetype.hpp"
#include "engine/utils/Defines.hpp"

#include "engine/components/CObject.hpp"
#include "engine/components/CPosition.hpp"
#include "engine/components/CSprite.hpp"
#include "engine/components/CVelocity.hpp"

typedef std::unordered_map<std::string, Engine::EntityId> EntityMap;

namespace Engine {
    class ECSManagerBase {
        public:
            ECSManagerBase();

            virtual ~ECSManagerBase();

            /**
             * @brief init ECSManagerBase Archetypes
             */
            void init();

            /**
             * @brief update the ECSManagerBase
             */
            void update();

            /**
             * @brief destroy the ECSManagerBase
             */
            void destroy();

            /**
             * @brief Add an Archetype to the ECSManagerBase
             * @tparam Archetype
             * @param name of the Archetype
             * @return Archetype
             */
            template <typename Archetype>
            Archetype *addArchetype(const std::string &name) {
                auto archetype = std::make_unique<std::any>(new Archetype());
                if (_archetypes.count(name) > 0)
                    _archetypes[name] = std::move(archetype);
                else
                    _archetypes.insert({name, std::move(archetype)});
                return getArchetype<Archetype>(name);
            }

            /**
             * @brief Get an Archetype from the ECSManagerBase
             * @param name of the Archetype
             * @return Archetype
             */
            template <typename Archetype>
            Archetype *getArchetype(const std::string &name) {
                if (_archetypes.count(name) > 0)
                    return std::any_cast<Archetype *>(*_archetypes[name]);
                return nullptr;
            }

        private:
            std::unordered_map<std::string, std::unique_ptr<std::any>> _archetypes;

        protected:
            /**
             * @brief init game Archetypes
             */
            virtual void initGame() = 0;

            /**
             * @brief update game Archetypes
             */
            virtual void updateGame() = 0;

            /**
             * @brief destroy game Archetypes
             */
            virtual void destroyGame() = 0;
    };
}
