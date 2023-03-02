/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CHealth.hpp
*/

#pragma once

#include "engine/components/ComponentBase.hpp"

class CCamera : public Engine::ComponentBase {
  public:
    /**
     * @brief Construct a new CCamera object
     */
    CCamera();

    /**
     * @brief Destroy the CCamera object
     */
    ~CCamera() override;

    /**
     * @brief Set the camera
     * @param Camera3D camera
     */
    void setCamera(Camera3D camera);

    /**
     * @brief Get the camera
     * @return Camera3D
     */
    [[nodiscard]] Camera3D getCamera() const;

    /**
     * @brief Set the position of the camera
     * @param Vector3 position
     */
    void setPosition(Vector3 position);

    /**
     * @brief Set the target of the camera
     * @param Vector3 target
     */
    void setTarget(Vector3 target);

    /**
     * @brief Set the up vector of the camera
     * @param Vector3 up
     */
    void setUp(Vector3 up);

    /**
     * @brief Set the fovy of the camera
     * @param float fovy
     */
    void setFovy(float fovy);

    /**
     * @brief Set the type of the camera
     * @param CameraType type
     */
    void setProjection(CameraProjection type);

  private:
    Camera3D _camera;
};
