/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Loading.cpp
*/

#include "game/Game.hpp"

void Game::loadEntities(const std::string &path) {
    if (path.empty())
        throw std::runtime_error("Invalid path to entities file");
    if (!FileExists(path.c_str()))
        throw std::runtime_error("Entities file not found");
    std::ifstream f(path);
    json data = json::parse(f);
    if (data.contains("objects")) {
        for (auto &object : data["objects"]) {
            loadObject(object);
        }
    }
    if (data.contains("ui")) {
        for (auto &ui : data["ui"]) {
            loadUI(ui);
        }
    }
}

void Game::loadObject(json &object) {
    if (!object.contains("name")) {
        TraceLog(LOG_WARNING, "Failed to load object: missing name");
        TraceLog(LOG_WARNING, "Skipping object: %s", object.dump().c_str());
        return;
    }
    std::string name = object["name"];
    auto entity = _pObjectArchetype->createEntity(Engine::CScale(1), Engine::CVelocity(Vector3Zero()), Engine::CObject(),
                                                  Engine::CPosition(Vector3Zero()), Engine::CRotation(Vector3Zero()), CCollider());
    _gameEntities[name] = entity;
    auto [cObject, cPosition, cScale, cVelocity, cRotation, cCollider] =
        _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CScale, Engine::CVelocity, Engine::CRotation, CCollider>(entity);
    cCollider.setActive(true);
    cCollider.setSize(1);
    cRotation.setActive(true);
    if (object.contains("model")) {
        auto model = object["model"];
        if (_models.contains(model)) {
            _pObjectArchetype->addComponent(entity, pModel(_models[model]));
            if (_shaders.contains("lighting"))
                _models[model]->setModelShader(_shaders["lighting"]->getShader());
        }
    }
    if (object.contains("tags")) {
        auto tags = object["tags"];
        for (auto &tag : tags) {
            cObject.setTag(tag);
        }
    }
    if (object.contains("position") && object["position"].size() == 3) {
        auto positionStr = object["position"];
        auto position = Vector3{positionStr[0], positionStr[1], positionStr[2]};
        cPosition.setPosition(position);
        cCollider.setPosition(position);
    }
    if (object.contains("rotation") && object["rotation"].size() == 3) {
        auto rotationStr = object["rotation"];
        auto rotation = Vector3{rotationStr[0], rotationStr[1], rotationStr[2]};
        cRotation.setRotation(rotation);
    }
    if (object.contains("setScale")) {
        auto scale = object["setScale"];
        cScale.setScale(scale);
        cCollider.setSize(Vector3{scale, scale, scale});
    }
    if (object.contains("animation")) {
        auto animation = object["animation"];
        if (_animations.contains(animation)) {
            _animations[animation]->setActive(true);
            _pObjectArchetype->addComponent(entity, pAnimation(_animations[animation]));
        }
    }
    if (object.contains("velocity") && object["velocity"].size() == 3) {
        auto velocityStr = object["velocity"];
        auto velocity = Vector3{velocityStr[0], velocityStr[1], velocityStr[2]};
        cVelocity.setVelocity(velocity);
        cVelocity.setActive(true);
    }
}

void Game::loadUI(json &ui) {
    if (!ui.contains("name")) {
        TraceLog(LOG_WARNING, "Failed to load UI: missing name");
        TraceLog(LOG_WARNING, "Skipping UI: %s", ui.dump().c_str());
        return;
    }
    std::string name = ui["name"];
    // typedef Engine::Archetype<Engine::CObject, CText, Engine::CPosition, Engine::CScale, CHandler, CBox, CColor> UIArchetype;

    auto entity = _pUIArchetype->createEntity(Engine::CObject(), Engine::CScale(1), Engine::CPosition(Vector3Zero()));
    _uiElements[name] = entity;
    auto [cObject, cPosition, cScale] = _pUIArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CScale>(entity);
    if (ui.contains("type")) {
        auto type = ui["type"];
        cObject.setTag(type);
        if (type == "text") {
            auto cText = CText();
            cText.setActive(true);
            if (ui.contains("text")) {
                auto text = ui["text"];
                cText.setText(text);
            }
            if (ui.contains("fontSize")) {
                auto fontSize = ui["fontSize"];
                cText.setFontSize(fontSize);
            }
            _pUIArchetype->addComponent(entity, CText(cText));
        }
        // TODO: add other types
    }
    if (ui.contains("position") && ui["position"].size() == 2) {
        auto positionStr = ui["position"];
        auto position = Vector3{positionStr[0], positionStr[1], 0};
        cPosition.setPosition(position);
    }
}
