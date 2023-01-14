/*
** EPITECH PROJECT, 2023
** Scene.hpp
** File description:
** [WIP]
*/

#ifndef RTYPE_SCENE_HPP
#define RTYPE_SCENE_HPP

#include <vector>

namespace rtype {
    class Scene {
    private:
        bool Hover{};
    public:
        Scene() = default;

        virtual ~Scene() = default;

        void AddEntity();
        void StartScene();
        void DrawScene();
        void Draw2DAssets();
        void UnloadScene();
    };
}

#endif //RTYPE_SCENE_HPP