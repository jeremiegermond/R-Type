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

void Game::loadAssetsGame(const json &assets) {
    if (assets.contains("emitters")) {
        for (auto &emitter : assets["emitters"]) {
            if (!emitter.contains("name")) {
                TraceLog(LOG_WARNING, "Failed to load emitter: missing name");
                TraceLog(LOG_WARNING, "Skipping emitter: %s", emitter.dump().c_str());
                continue;
            }
            std::string name = emitter["name"];
            auto settings = loadParticleEmitterSettings(emitter);
            _emitters[name] = CParticleEmitter(settings);
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
    auto entity = _pObjectArchetype.createEntity(Engine::CScale(1), Engine::CVelocity(Vector3Zero()), Engine::CObject(),
                                                 Engine::CPosition(Vector3Zero()), Engine::CRotation(Vector3Zero()), CCollider());
    _gameEntities[name] = entity;
    auto [cObject, cPosition, cScale, cVelocity, cRotation, cCollider] =
        _pObjectArchetype.getComponent<Engine::CObject, Engine::CPosition, Engine::CScale, Engine::CVelocity, Engine::CRotation, CCollider>(entity);
    cCollider.setActive(true);
    cCollider.setSize(1);
    cRotation.setActive(true);
    if (object.contains("model")) {
        auto model = object["model"];
        if (_models.contains(model)) {
            _pObjectArchetype.addComponent(entity, pModel(_models[model]));
            if (_shaders.contains("lighting"))
                _models[model]->setModelShader(_shaders["lighting"]->getShader());
        }
    }
    if (object.contains("tags")) {
        auto tags = object["tags"];
        for (auto &tag : tags) {
            cObject.setTag(tag);
        }
        if (cObject.hasTag("player")) {
            CText cText;
            cText.setFontSize(4);
            cText.setOffset({0, 1, 0});
            _pObjectArchetype.addComponent(entity, CText(cText));
            cObject.setTag("named");
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
            _pObjectArchetype.addComponent(entity, pAnimation(_animations[animation]));
        }
    }
    if (object.contains("velocity") && object["velocity"].size() == 3) {
        auto velocityStr = object["velocity"];
        auto velocity = Vector3{velocityStr[0], velocityStr[1], velocityStr[2]};
        cVelocity.setVelocity(velocity);
        cVelocity.setActive(true);
    }
    if (object.contains("emitterBase")) {
        auto emitterName = object["emitterBase"];
        if (!_emitters.contains(emitterName)) {
            TraceLog(LOG_WARNING, "Failed to load emitter: missing name");
            TraceLog(LOG_WARNING, "Skipping emitter: %s", object.dump().c_str());
            return;
        }
        auto emitter = CParticleEmitter(_emitters[emitterName]);
        if (object.contains("emitterSettings")) {
            auto settings = loadParticleEmitterSettings(object["emitterSettings"], emitter.getSettings());
            emitter.setSettings(settings);
        }
        emitter.setActive(true);
        _pObjectArchetype.addComponent(entity, CParticleEmitter(emitter));
        cObject.setTag("emitter");
    }
}

void Game::loadUI(json &ui) {
    if (!ui.contains("name")) {
        TraceLog(LOG_WARNING, "Failed to load UI: missing name");
        TraceLog(LOG_WARNING, "Skipping UI: %s", ui.dump().c_str());
        return;
    }
    std::string name = ui["name"];

    auto entity = _pUIArchetype.createEntity(Engine::CObject(), Engine::CScale(1), Engine::CPosition(Vector3Zero()), CText());
    _uiElements[name] = entity;
    auto [cObject, cPosition, cScale, cText] = _pUIArchetype.getComponent<Engine::CObject, Engine::CPosition, Engine::CScale, CText>(entity);
    if (ui.contains("type")) {
        auto type = ui["type"];
        cObject.setTag(type);
        if (type == "button" || type == "input") {
            cObject.setTag("text");
            _pUIArchetype.addComponent(entity, CBox());
            _pUIArchetype.addComponent(entity, CColor());
            auto [cBox, cColor] = _pUIArchetype.getComponent<CBox, CColor>(entity);
            if (ui.contains("size") && ui["size"].size() == 2) {
                auto sizeStr = ui["size"];
                auto size = Vector2{sizeStr[0], sizeStr[1]};
                cBox.setSize(size);
            }
            if (ui.contains("color") && ui["color"].size() == 4) {
                auto colorStr = ui["color"];
                auto color = Color{colorStr[0], colorStr[1], colorStr[2], colorStr[3]};
                cColor.setColor(color);
            }
        }
    }
    if (ui.contains("text")) {
        auto text = ui["text"];
        cText.setText(text);
        cText.setActive(true);
    }
    if (ui.contains("fontSize")) {
        auto fontSize = ui["fontSize"];
        cText.setFontSize(fontSize);
    }
    if (ui.contains("position") && ui["position"].size() == 2) {
        auto positionStr = ui["position"];
        auto position = Vector3{positionStr[0], positionStr[1], 0};
        cPosition.setPosition(position);
    }
    if (ui.contains("size") && ui["size"].size() == 2 && (cObject.hasTag("button") || cObject.hasTag("input"))) {
        auto sizeStr = ui["size"];
        auto size = Vector2{sizeStr[0], sizeStr[1]};
        auto [cBox, cColor] = _pUIArchetype.getComponent<CBox, CColor>(entity);
        cBox.setSize(size);
    }
    if (ui.contains("tags")) {
        auto tags = ui["tags"];
        for (auto &tag : tags) {
            cObject.setTag(tag);
        }
    }
}

ParticleEmitterSettings Game::loadParticleEmitterSettings(const json &emitter, ParticleEmitterSettings settings) {
    if (emitter.contains("offset") && emitter["offset"].size() == 3) {
        settings.offset = Vector3{emitter["offset"][0], emitter["offset"][1], emitter["offset"][2]};
    }
    if (emitter.contains("velocity") && emitter["velocity"].size() == 3) {
        settings.velocity = Vector3{emitter["velocity"][0], emitter["velocity"][1], emitter["velocity"][2]};
    }
    if (emitter.contains("acceleration") && emitter["acceleration"].size() == 3) {
        settings.acceleration = Vector3{emitter["acceleration"][0], emitter["acceleration"][1], emitter["acceleration"][2]};
    }
    if (emitter.contains("velocityVariation") && emitter["velocityVariation"].size() == 3) {
        settings.velocityVariation = Vector3{emitter["velocityVariation"][0], emitter["velocityVariation"][1], emitter["velocityVariation"][2]};
    }
    if (emitter.contains("accelerationVariation") && emitter["accelerationVariation"].size() == 3) {
        settings.accelerationVariation =
            Vector3{emitter["accelerationVariation"][0], emitter["accelerationVariation"][1], emitter["accelerationVariation"][2]};
    }
    if (emitter.contains("offsetVariation") && emitter["offsetVariation"].size() == 3) {
        settings.offsetVariation = Vector3{emitter["offsetVariation"][0], emitter["offsetVariation"][1], emitter["offsetVariation"][2]};
    }
    if (emitter.contains("spawnRate")) {
        settings.spawnRate = emitter["spawnRate"];
    }
    if (emitter.contains("lifeTime")) {
        settings.lifeTime = emitter["lifeTime"];
    }
    if (emitter.contains("lifeTimeVariation")) {
        settings.lifeTimeVariation = emitter["lifeTimeVariation"];
    }
    if (emitter.contains("size")) {
        settings.size = emitter["size"];
    }
    if (emitter.contains("sizeVariation")) {
        settings.sizeVariation = emitter["sizeVariation"];
    }
    if (emitter.contains("color") && emitter["color"].size() == 4) {
        settings.color = Color{emitter["color"][0], emitter["color"][1], emitter["color"][2], emitter["color"][3]};
    }
    if (emitter.contains("endColor") && emitter["endColor"].size() == 4) {
        settings.endColor = Color{emitter["endColor"][0], emitter["endColor"][1], emitter["endColor"][2], emitter["endColor"][3]};
    }
    if (emitter.contains("maxParticles")) {
        settings.maxParticles = emitter["maxParticles"];
    }
    if (emitter.contains("spawnAmount")) {
        settings.spawnAmount = emitter["spawnAmount"];
    }
    return settings;
}
