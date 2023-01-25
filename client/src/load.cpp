/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Load.cpp by thibb1
*/

#include "rtype-client.hpp"

using json = nlohmann::json;

void SetModelTexture(Model *model, int mapType, Texture2D texture) {
    for (int i = 0; i < model->materialCount; i++) {
        SetMaterialTexture(&model->materials[i], mapType, texture);
    }
}

void loadAssets(std::unordered_map<std::string, MyObject> &objects, std::unordered_map<std::string, MyMusic> &musics,
                std::unordered_map<std::string, Sound> &sounds, std::unordered_map<std::string, MyTexture> &textures,
                std::unordered_map<std::string, Shader> &shaders, const std::string &assetsPath) {
    // open json file
    std::ifstream f(assetsPath);
    json data = json::parse(f);

    // Load models
    // mutex.lock();
    std::cout << "Loading objects..." << std::endl;
    for (auto &object : data["models"]) {
        MyObject myObject;
        myObject.position = Vector3Zero();
        myObject.rotation = Vector3Zero();
        myObject.rotationGoal = Vector3Zero();
        myObject.scale = 1;
        std::string name = object["name"];
        std::string path = object["path"];
        myObject.model = LoadModel(path.c_str());
        if (object.contains("scale")) {
            myObject.scale = object["scale"];
            // myObject.model.transform = MatrixScale(myObject.scale, myObject.scale, myObject.scale);
            // myObject.scale = 1;
        }
        if (object.contains("rotation") && object["rotation"].size() == 3) {
            myObject.rotation = {object["rotation"][0], object["rotation"][1], object["rotation"][2]};
            myObject.rotationGoal = myObject.rotation;
            myObject.model.transform = MatrixRotateXYZ(myObject.rotation);
        }
        if (object.contains("position") && object["position"].size() == 3) {
            myObject.position = {object["position"][0], object["position"][1], object["position"][2]};
        }
        myObject.animationCount = 0;
        if (object.contains("animation")) {
            std::string animationPath = object["animation"]["path"];
            myObject.animations = LoadModelAnimations(animationPath.c_str(), &myObject.animationCount);
        }
        if (object.contains("textures")) {
            for (auto &texture : object["textures"]) {
                std::string type = texture["type"];
                std::string pathTexture = texture["path"];
                int materialIndex = -1;
                if (texture.contains("materialIndex"))
                    materialIndex = texture["materialIndex"];
                TraceLog(LOG_INFO, "Loading texture %s", pathTexture.c_str());
                myObject.textures.push_back(new Texture2D(LoadTexture(pathTexture.c_str())));
                if (type == "diffuse") {
                    if (materialIndex == -1)
                        SetModelTexture(&myObject.model, MATERIAL_MAP_DIFFUSE, *myObject.textures.back());
                    else
                        SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_DIFFUSE, *myObject.textures.back());
                } else if (type == "normal") {
                    if (materialIndex == -1)
                        SetModelTexture(&myObject.model, MATERIAL_MAP_NORMAL, *myObject.textures.back());
                    else
                        SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_NORMAL, *myObject.textures.back());
                } else if (type == "metallic") {
                    if (materialIndex == -1)
                        SetModelTexture(&myObject.model, MATERIAL_MAP_METALNESS, *myObject.textures.back());
                    else
                        SetMaterialTexture(&myObject.model.materials[materialIndex], MATERIAL_MAP_METALNESS, *myObject.textures.back());
                }
            }
        }
        objects[name] = myObject;
    }
    std::cout << "Loaded " << objects.size() << " objects" << std::endl;

    // Load musics
    for (auto &music : data["musics"]) {
        MyMusic myMusic;
        std::string name = music["name"];
        std::string path = music["path"];
        myMusic.name = name;
        myMusic.music = LoadMusicStream(path.c_str());
        myMusic.music.looping = true;
        musics[name] = myMusic;
        // musics[name].looping = true;
    }

    // Load sounds
    for (auto &sound : data["sounds"]) {
        std::string name = sound["name"];
        std::string path = sound["path"];
        sounds[name] = LoadSound(path.c_str());
    }

    // Load sprites
    for (auto &sprite : data["sprites"]) {
        std::string name = sprite["name"];
        std::string path = sprite["path"];
        int columns = sprite["columns"];
        int rows = sprite["rows"];
        MyTexture myTexture;
        myTexture.texture = LoadTexture(path.c_str());
        myTexture.columns = columns;
        myTexture.rows = rows;
        myTexture.scale = {1, 1};
        if (sprite.find("scale") != sprite.end() && sprite["scale"].size() == 2) {
            myTexture.scale = {sprite["scale"][0], sprite["scale"][1]};
        }
        textures[name] = myTexture;
    }
    std::cout << "Loaded " << textures.size() << " sprites" << std::endl;

    // Load shaders
    for (auto &shader : data["shaders"]) {
        std::string name = shader["name"];
        // check vertex
        Shader s;
        std::string pathFragment = shader["path_fragment"];
        if (shader.contains("path_vertex")) {
            std::string pathVertex = shader["path_vertex"];
            std::cout << "Loading shader " << name << " with vertex shader " << pathVertex << " and fragment shader " << pathFragment << std::endl;
            s = LoadShader(pathVertex.c_str(), pathFragment.c_str());
        } else {
            s = LoadShader(nullptr, pathFragment.c_str());
        }
        if (shader.contains("locs")) {
            for (auto &loc : shader["locs"]) {
                std::string nameLoc = loc["name"];
                std::string from = loc["from"];
                if (nameLoc == "loc_matrix_model") {
                    s.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(s, from.c_str());
                } else if (nameLoc == "loc_vector_light") {
                    s.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(s, from.c_str());
                }
            }
        }
        shaders[name] = s;
    }
    std::cout << "Loaded " << shaders.size() << " shaders" << std::endl;
    //  Unload json file
    f.close();
}
