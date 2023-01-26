/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** engine.hpp by thibb1
*/

#ifndef RTYPE_ENGINE_HPP
#define RTYPE_ENGINE_HPP
#include "rtype-client.hpp"

using json = nlohmann::json;

class Engine {
  private:
    std::unordered_map<std::string, GameObject *> _objects;
    std::unordered_map<std::string, MyMusic> _musics;
    std::unordered_map<std::string, Sound> _sounds;
    std::unordered_map<std::string, MyTexture> _textures;
    std::unordered_map<std::string, Shader> _shaders;
    std::vector<Light> _lights;
    std::set<int> _availableLights;
    std::vector<Particle> _particles;
    std::vector<Particle2D> _particles2D;
    std::vector<Bullet> _bullets;
    std::vector<Slider> _sliders;
    std::string _musicPlaying;
    Camera3D _camera{};
    Model _companionCube{};

  public:
    explicit Engine(const std::string &assetsPath);

    ~Engine();

    void loadAssets(const std::string &assetsPath);
    void loadShaders(json data);
    void loadTextures(json data);
    void loadMusics(json data);
    void loadSounds(json data);
    void loadObjects(json data);

    void unloadAssets();
    void clearSliders();

    bool updateSliders();
    void updateMusic();
    void updateParticles();
    void updateLights();
    void updateObjects();
    void updateCamera();
    void updateShaderLocView(const std::string &shaderName);
    void updateParticles2D();
    void updateBullets();

    void drawObject(const std::string &name, Vector3 offset = Vector3Zero());
    void drawObjects();
    void drawParticles();
    void drawLightsSpheres();
    void drawBullets();
    void drawParticles2D();
    void drawSliders();

    void playMusic(const std::string &name);
    void setShaderMode(const std::string &name);

    void addSlider(const std::string &name, Rectangle bounds, float *value, float minValue, float maxValue);
    int addLight(Vector3 position, float intensity = .2, Color color = WHITE);
    void addBullet(Vector3 position, Vector3 velocity);
    void addParticle2D(const std::string &textureName, Vector3 position, float rotation = 0);
    void addParticle(Vector3 position, float scale = 1, Color color = WHITE);

    GameObject *getObject(const std::string &name);
    std::unordered_map<std::string, GameObject *> getObjects();
    Light *getLight(int index);
    Camera3D *getCamera();

    void setShaderObject(const std::string &name, const std::string &shaderName);

    bool isInScreen(Vector3 position, float offset = 1000);
};
#endif // RTYPE_ENGINE_HPP
