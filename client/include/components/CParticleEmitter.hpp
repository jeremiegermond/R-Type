/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CParticleEmitter.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

typedef struct ParticleEmitterSettings {
    Vector3 position;
    Vector3 offset;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 velocityVariation;
    Vector3 accelerationVariation;
    Vector3 offsetVariation;
    float spawnRate;
    float lifeTime;
    float lifeTimeVariation;
    float size;
    float sizeVariation;
    Color color;
    Color endColor;
    int maxParticles;
    int spawnAmount;
} ParticleEmitterSettings;

typedef struct Particle {
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Color color;
    float size;
    float lifeTimeLeft;
} Particle;

Color colorLerp(Color start, Color end, float d);

class CParticleEmitter : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CParticleEmitter object
     */
    CParticleEmitter();

    /**
     * @brief Construct a new CParticleEmitter object
     * @param ParticleEmitterSettings settings
     */
    explicit CParticleEmitter(const ParticleEmitterSettings &settings);

    /**
     * @brief Destroy the CParticleEmitter object
     */
    ~CParticleEmitter() override = default;

    /**
     * @brief Set the ParticleEmitter settings
     * @param ParticleEmitterSettings settings
     */
    void setSettings(const ParticleEmitterSettings &settings);

    /**
     * @brief Get the ParticleEmitter settings
     * @return ParticleEmitterSettings
     */
    ParticleEmitterSettings getSettings() const;

    /**
     * @brief Set the ParticleEmitter position
     * @param Vector3 position
     */
    void setPosition(const Vector3 &position);

    /**
     * @brief doUpdate the ParticleEmitter
     */
    void doUpdate() override;

    /**
     * @brief draw the Particles
     */
    void draw();

    /**
     * @brief Add a particle to the ParticleEmitter
     */
    void addParticle();

    /**
     * @brief Update a particle
     * @param float frameTime
     * @param Particle particle
     */
    void updateParticle(float frameTime, Particle &particle);

    /**
     * @brief Get random float
     * @param float value
     * @return float
     */
    float getRandomFloat(float value);

    /**
     * @brief Get random Vector3
     * @param Vector3 value
     * @return Vector3
     */
    Vector3 getRandomVector3(Vector3 value);

  private:
    ParticleEmitterSettings _settings;
    std::vector<Particle> _particles;
    float _spawnTimer;
    std::default_random_engine _generator;
};
