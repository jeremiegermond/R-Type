/*
** EPITECH PROJECT, 2023
** SceneManager.hpp
** File description:
** [WIP]
*/
#ifndef RTYPE_SCENEMANAGER_HPP
#define RTYPE_SCENEMANAGER_HPP

#include "Scene.hpp"

namespace rtype {
    class Game;
    class SceneManager {
    private:
        std::vector<Scene *> scenes;
        Game *gameRef{};

    public:
        SceneManager() = default;
        explicit SceneManager(Game *pGame);

        virtual ~SceneManager() = default;

        void CreateMenuScene();

        void CreateLevelScene();

        Scene *GetScene(int sceneId);

        void CreateUiScene();

        void CreateSettingsScene();

        void CreateHowToPlayScene();

        void CreateVictoryOrDefeatScene();

        void CreateTieScene();

        void display_loading(int percentage) {
        }
    };
}

#endif //RTYPE_SCENEMANAGER_HPP