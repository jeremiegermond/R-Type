/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CParticleEmitter.cpp
*/

#include "components/CParticleEmitter.hpp"

CParticleEmitter::CParticleEmitter() : _settings({0}), _spawnTimer(0) { _generator.seed(std::random_device()()); }

CParticleEmitter::CParticleEmitter(const ParticleEmitterSettings &settings) : CParticleEmitter() { setSettings(settings); }

void CParticleEmitter::setSettings(const ParticleEmitterSettings &settings) { _settings = settings; }

void CParticleEmitter::setPosition(const Vector3 &position) { _settings.position = position; }

void CParticleEmitter::doUpdate() {
    if (_settings.spawnRate > 0) {
        _spawnTimer += GetFrameTime();
        if (_spawnTimer >= _settings.spawnRate && _particles.size() < _settings.maxParticles) {
            _spawnTimer = 0;
            for (int i = 0; i < _settings.spawnAmount; i++)
                addParticle();
        }
    }
    auto frameTime = GetFrameTime();
    for (auto it = _particles.begin(); it != _particles.end();) {
        if (it->lifeTimeLeft <= 0) {
            it = _particles.erase(it);
        } else {
            updateParticle(frameTime, *it);
            it++;
        }
    }
}

void CParticleEmitter::draw() {
    for (auto &particle : _particles) {
        DrawCubeV(particle.position, {particle.size, particle.size, particle.size}, particle.color);
    }
}

void CParticleEmitter::updateParticle(float frameTime, Particle &particle) {
    particle.lifeTimeLeft -= frameTime;
    if (particle.lifeTimeLeft < 0)
        particle.lifeTimeLeft = 0;
    auto acceleration = Vector3Scale(particle.acceleration, frameTime);
    particle.velocity = Vector3Add(particle.velocity, acceleration);
    auto velocity = Vector3Scale(particle.velocity, frameTime);
    particle.position = Vector3Add(particle.position, velocity);
    auto color = _settings.color;
    particle.color = colorLerp(color, _settings.endColor, 1 - particle.lifeTimeLeft / _settings.lifeTime);
}

void CParticleEmitter::addParticle() {
    auto position = Vector3Add(_settings.position, _settings.offset);
    position = Vector3Add(position, getRandomVector3(_settings.offsetVariation));
    auto velocity = Vector3Add(_settings.velocity, getRandomVector3(_settings.velocityVariation));
    auto acceleration = Vector3Add(_settings.acceleration, getRandomVector3(_settings.accelerationVariation));
    auto color = _settings.color;
    auto size = _settings.size + getRandomFloat(_settings.sizeVariation);
    auto lifeTime = _settings.lifeTime + getRandomFloat(_settings.lifeTimeVariation);
    _particles.push_back({position, velocity, acceleration, color, size, lifeTime});
}

float CParticleEmitter::getRandomFloat(float value) {
    std::uniform_real_distribution<float> distribution(-value, value);
    return distribution(_generator);
}

Vector3 CParticleEmitter::getRandomVector3(Vector3 value) {
    auto x = getRandomFloat(value.x);
    auto y = getRandomFloat(value.y);
    auto z = getRandomFloat(value.z);
    return {x, y, z};
}

ParticleEmitterSettings CParticleEmitter::getSettings() const { return _settings; }

Color colorLerp(Color start, Color end, float d) {
    auto r = start.r + (end.r - start.r) * d;
    auto g = start.g + (end.g - start.g) * d;
    auto b = start.b + (end.b - start.b) * d;
    auto a = start.a + (end.a - start.a) * d;
    r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
    g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
    b = (b < 0) ? 0 : ((b > 255) ? 255 : b);
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
}
