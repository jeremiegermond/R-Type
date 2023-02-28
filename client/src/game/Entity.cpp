/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity.cpp
*/

#include "game/Game.hpp"

void Game::updateEntity(Engine::EntityId id) {
    auto [cPosition, cVelocity, cRotation, cCollider] =
        _pObjectArchetype->getComponent<Engine::CPosition, Engine::CVelocity, Engine::CRotation, CCollider>(id);
    cRotation.update();
    cVelocity.update();
    cPosition.addPosition(cVelocity.getVelocity());
    cCollider.setPosition(cPosition.getPosition());
    auto [pModel, pAnimation] = _pObjectArchetype->getComponent<std::shared_ptr<Engine::CModel>, std::shared_ptr<Engine::CAnimation>>(id);
    if (pAnimation != nullptr) {
        pAnimation->update();
        auto animation = pAnimation->getAnimation();
        UpdateModelAnimation(pModel->getModel(), animation, pAnimation->getCurrentFrame());
    }
}

void Game::drawEntity(Engine::EntityId id, Vector3 offset) {
    auto cModel = _pObjectArchetype->getComponent<pModel>(id);
    if (cModel == nullptr)
        return;
    auto position = _pObjectArchetype->getComponent<Engine::CPosition>(id).getPosition();
    auto rotation = _pObjectArchetype->getComponent<Engine::CRotation>(id).getRotation();
    auto scale = _pObjectArchetype->getComponent<Engine::CScale>(id).getScale();
    auto model = cModel->getModel();
    model.transform = MatrixRotateXYZ(rotation);
    auto cCollider = _pObjectArchetype->getComponent<CCollider>(id);
    DrawBoundingBox(cCollider.getBox(), RED);
    DrawModel(model, Vector3Add(position, offset), scale, WHITE);
}

void Game::loadEntities(const std::string &path) {
    if (path.empty())
        throw std::runtime_error("Invalid path to entities file");
    if (!FileExists(path.c_str()))
        throw std::runtime_error("Entities file not found");
    std::ifstream f(path);
    json data = json::parse(f);
    if (data.contains("objects")) {
        for (auto &object : data["objects"]) {
            if (!object.contains("name")) {
                TraceLog(LOG_WARNING, "Failed to load object: missing name");
                TraceLog(LOG_WARNING, "Skipping object: %s", object.dump().c_str());
                continue;
            }
            std::string name = object["name"];
            auto entity = _pObjectArchetype->createEntity(Engine::CScale(1), Engine::CVelocity(Vector3Zero()), Engine::CObject(),
                                                          Engine::CPosition(Vector3Zero()), Engine::CRotation(Vector3Zero()), CCollider());
            _gameEntities[name] = entity;
            auto [cObject, cPosition, cScale, cVelocity, cRotation, cCollider] =
                _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CScale, Engine::CVelocity, Engine::CRotation, CCollider>(
                    entity);
            cCollider.setActive(true);
            cCollider.setSize(1);
            cRotation.setActive(true);
            if (object.contains("model")) {
                auto model = object["model"];
                if (!_models.contains(model))
                    continue;
                _pObjectArchetype->addComponent(entity, pModel(_models[model]));
                if (_shaders.contains("lighting"))
                    _models[model]->setModelShader(_shaders["lighting"]->getShader());
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
            if (object.contains("scale")) {
                auto scale = object["scale"];
                cScale.setScale(scale);
                cCollider.setSize(Vector3{scale, scale, scale});
            }
            if (object.contains("animation")) {
                auto animation = object["animation"];
                if (!_animations.contains(animation))
                    continue;
                _animations[animation]->setActive(true);
                _pObjectArchetype->addComponent(entity, pAnimation(_animations[animation]));
            }
            if (object.contains("velocity") && object["velocity"].size() == 3) {
                auto velocityStr = object["velocity"];
                auto velocity = Vector3{velocityStr[0], velocityStr[1], velocityStr[2]};
                cVelocity.setVelocity(velocity);
                cVelocity.setActive(true);
            }
        }
    }
}

void Game::addEnemy(int id, Vector3 position, Vector3 velocity, int hp) {
    auto enemy = _enemies.contains(id) ? _enemies[id]
                                       : _pObjectArchetype->createEntity(Engine::CScale(0.1), Engine::CPosition(), Engine::CVelocity(),
                                                                         Engine::CObject(), Engine::CRotation({0, 3, 0}), CHealth(), CCollider());
    auto [cObject, cPosition, cVelocity, cHealth, cCollider] =
        _pObjectArchetype->getComponent<Engine::CObject, Engine::CPosition, Engine::CVelocity, CHealth, CCollider>(enemy);
    cObject.setActive(true);
    cPosition.setPosition(position);
    cVelocity.setVelocity(velocity);
    cVelocity.setActive(true);
    cHealth.setHealth(hp);
    cCollider.setActive(true);
    cCollider.setSize(1);
    cCollider.setPosition(position);
    if (_models.contains("E002")) {
        _pObjectArchetype->addComponent(enemy, pModel(_models["E002"]));
    }
    _enemies[id] = enemy;
}
