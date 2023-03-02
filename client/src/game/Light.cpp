/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Light.cpp
*/

#include "game/Game.hpp"

int Game::addLight(Vector3 position, Color color, float intensity) {
    int idx = *_lightIds.begin();
    if (idx == -1 || !_shaders.contains("lighting"))
        return -1;
    Light light(idx, LIGHT_POINT, position, Vector3Zero(), color, *_shaders["lighting"]->getShader(), intensity);
    _lights.emplace_back(light);
    _lightIds.erase(idx);
    return idx;
}

void Game::updateLights() {
    Shader shader = *_shaders["lighting"]->getShader();
    for (auto &light : _lights) {
        light.UpdateLightValues(shader);
    }
    for (auto it = _lights.begin(); it != _lights.end();) {
        if (!it->isEnabled()) {
            _lightIds.emplace(it->getIndex());
            it = _lights.erase(it);
        } else {
            ++it;
        }
    }
    auto position = _pCameraArchetype->getComponent<CCamera>(_camera).getCamera().position;
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &position, SHADER_UNIFORM_VEC3);
}
