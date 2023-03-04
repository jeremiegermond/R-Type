/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** CCamera.cpp
*/

#include "components/CCamera.hpp"

CCamera::CCamera() : _camera({0}) {}

CCamera::~CCamera() {}

void CCamera::setCamera(Camera3D camera) { _camera = camera; }

Camera3D CCamera::getCamera() const { return _camera; }

void CCamera::setPosition(Vector3 position) { _camera.position = position; }

void CCamera::setTarget(Vector3 target) { _camera.target = target; }

void CCamera::setUp(Vector3 up) { _camera.up = up; }

void CCamera::setFovy(float fovy) { _camera.fovy = fovy; }

void CCamera::setProjection(CameraProjection type) { _camera.projection = type; }
