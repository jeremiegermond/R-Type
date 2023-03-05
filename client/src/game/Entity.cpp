/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Entity.cpp
*/

#include "game/Game.hpp"

void Game::updateEntity(Engine::EntityId id) {
    auto [cPosition, cVelocity, cRotation, cCollider] =
        _objectArchetype.getComponent<Engine::CPosition, Engine::CVelocity, Engine::CRotation, CCollider>(id);
    cRotation.update();
    cVelocity.update();
    cPosition.addPosition(cVelocity.getVelocity());
    cCollider.setPosition(cPosition.getPosition());
    auto [pModel, pAnimation] = _objectArchetype.getComponent<std::shared_ptr<Engine::CModel>, std::shared_ptr<Engine::CAnimation>>(id);
    if (pAnimation != nullptr) {
        pAnimation->update();
        auto animation = pAnimation->getAnimation();
        UpdateModelAnimation(pModel->getModel(), animation, pAnimation->getCurrentFrame());
    }
    if (_objectArchetype.getComponent<Engine::CObject>(id).hasTag("emitter")) {
        auto [cObject, cEmitter] = _objectArchetype.getComponent<Engine::CObject, CParticleEmitter>(id);

        cEmitter.setPosition(cPosition.getPosition());
        cEmitter.doUpdate();
    }
}

void Game::drawEntity(Engine::EntityId id, Vector3 offset) {
    auto [cObject, cModel, cPosition, cRotation, cScale, cAnimation, cVelocity, cHealth, cCollider, cEmitter, cText] =
        _objectArchetype.getComponent<Engine::CObject, std::shared_ptr<Engine::CModel>, Engine::CPosition, Engine::CRotation, Engine::CScale,
                                      std::shared_ptr<Engine::CAnimation>, Engine::CVelocity, CHealth, CCollider, CParticleEmitter, CText>(id);
    if (cModel == nullptr)
        return;
    auto position = _objectArchetype.getComponent<Engine::CPosition>(id).getPosition();
    auto rotation = _objectArchetype.getComponent<Engine::CRotation>(id).getRotation();
    auto scale = _objectArchetype.getComponent<Engine::CScale>(id).getScale();
    auto model = cModel->getModel();
    model.transform = MatrixRotateXYZ(rotation);
    if (cObject.hasTag("emitter")) {
        auto cEmitter = _objectArchetype.getComponent<CParticleEmitter>(id);
        cEmitter.draw();
    }
    DrawModel(model, Vector3Add(position, offset), scale, WHITE);
    if (cObject.hasTag("named")) {
        auto text = _objectArchetype.getComponent<CText>(id);
        auto textSize = measureText3D(text.getText(), text.getFontSize());
        position.x -= textSize.x * 0.5f;
        rlPushMatrix();
        rlRotatef(90, 1, 0, 0);
        auto textPos = Vector3Add(Vector3Add(position, offset), text.getOffset());
        drawText3D(text.getText(), textPos, text.getFontSize(), WHITE);
        rlPopMatrix();
    }
}

void Game::drawTexture(Engine::EntityId id, Vector3 offset) {
    auto [cObject, cTexture, cAnimatedSprite, cPosition] =
        _pSpriteArchetype.getComponent<Engine::CObject, pTexture, CAnimatedSprite, Engine::CPosition>(id);
    if (cObject.isActive()) {
        // get the texture
        auto texture = cTexture->getTexture();
        // get the source rectangle
        auto source = cAnimatedSprite.getSourceRec();
        // get the scale
        auto scale = cAnimatedSprite.getScale();
        // get the rotation
        auto rotation = cAnimatedSprite.getRotation();
        // get the position
        auto position = cPosition.getPosition();
        position.z += 1;
        // get the camera
        auto camera = _pCameraArchetype.getComponent<CCamera>(_camera).getCamera();
        // draw the billboard pro
        DrawBillboardPro(camera, *texture, source, position, {0, 1, 0}, scale, Vector2Zero(), rotation, WHITE);
    }
}

void Game::addEnemy(int id, Vector3 position, Vector3 velocity, int hp) {
    if (_enemies.contains(id)) {
        _objectArchetype.removeEntity(_enemies[id]);
    }
    Engine::CObject enemyObject;
    Engine::CPosition enemyPosition;
    Engine::CVelocity enemyVelocity;
    Engine::CRotation enemyRotation;
    Engine::CScale enemyScale;
    CHealth enemyHealth;
    CCollider enemyCollider;
    pModel enemyModel = nullptr;
    CParticleEmitter enemyEmitter;
    enemyObject.setActive(true);
    enemyPosition.setPosition(position);
    enemyVelocity.setVelocity(velocity);
    enemyVelocity.setActive(true);
    enemyRotation.setRotation({0, 3, 0});
    enemyScale.setScale(0.1);
    enemyHealth.setHealth(hp);
    enemyCollider.setActive(true);
    enemyCollider.setSize(1);
    enemyCollider.setPosition(position);
    if (_models.contains("E002")) {
        enemyModel = _models["E002"];
    }
    if (_emitters.contains("enemy_trail")) {
        enemyEmitter.setSettings(_emitters["enemy_trail"].getSettings());
        enemyEmitter.setActive(true);
        enemyObject.setTag("emitter");
    }
    auto enemy = _objectArchetype.createEntity(enemyObject, enemyModel, enemyPosition, enemyRotation, enemyScale, nullptr, enemyVelocity,
                                               enemyHealth, enemyCollider, enemyEmitter, CText());
    _enemies[id] = enemy;
}

void Game::addAnimatedSprite(const std::string &name, Vector3 position) {
    if (!_textures.contains(name))
        return;
    auto textureP = _textures[name];
    Engine::CObject sprite;
    Engine::CPosition spritePosition;
    CAnimatedSprite spriteAnimatedSprite;
    auto texture = textureP->getTexture();
    sprite.setActive(true);
    spritePosition.setPosition(position);
    spriteAnimatedSprite.setTextureSize({(float)texture->width, (float)texture->height}, textureP->getRows(), textureP->getColumns());
    spriteAnimatedSprite.setScale(textureP->getScale());
    spriteAnimatedSprite.setActive(true);
    auto spriteEntity = _pSpriteArchetype.createEntity(sprite, spritePosition, spriteAnimatedSprite, textureP);
    _animatedSprites.push_back(spriteEntity);
}

void Game::updateTextures() {
    for (auto it = _animatedSprites.begin(); it != _animatedSprites.end();) {
        auto [cObject, cAnimatedSprite] = _pSpriteArchetype.getComponent<Engine::CObject, CAnimatedSprite>(*it);
        if (!cAnimatedSprite.getEnabled()) {
            it = _animatedSprites.erase(it);
        } else {
            cAnimatedSprite.update();
            ++it;
        }
    }
}
