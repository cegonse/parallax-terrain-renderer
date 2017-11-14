// Terrain Viewer
//
// Camera, file: Camera.cpp
//
// - Implementation of the Camera class.
//
// Copyright (c) César González Segura 2015, ETSE-UV
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.
//

#include "Camera.h"

Camera::Camera(const glm::vec3 & origin, const glm::vec3 & target, float fov, bool ortho, int x, int y)
{
    _origin = origin;
    _target = target;
    _fov = fov;
    _orthographic = ortho;
    _viewportWidth = x;
    _viewportHeight = y;

    if (_orthographic)
    {
        _projectionMatrix = glm::ortho(0.0f, (float) _viewportHeight, 0.0f, (float) _viewportWidth, 0.01f, 100.0f);
    }
    else
    {
        float aspectRatio = ((float) _viewportWidth) / ((float) _viewportHeight);
        _projectionMatrix = glm::perspective(_fov, aspectRatio, 0.01f, 100.0f);
    }

    _viewMatrix = glm::lookAt(_origin, _target, glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera()
{

}

void Camera::SetTarget(const glm::vec3 & target)
{
    _target = target;
    _viewMatrix = glm::lookAt(_origin, _target, glm::vec3(0.0f, 1.0f, 0.0f));
	_viewMatrix = _localRotation * _viewMatrix;
}

void Camera::SetOrigin(const glm::vec3 & origin)
{
    _origin = origin;
    _viewMatrix = glm::lookAt(_origin, _target, glm::vec3(0.0f, 1.0f, 0.0f));
	_viewMatrix = _localRotation * _viewMatrix;
}

void Camera::SetFOV(float fov)
{
    _fov = fov;

    if (!_orthographic)
    {
        float aspectRatio = ((float) _viewportWidth) / ((float) _viewportHeight);
        _projectionMatrix = glm::perspective(_fov, aspectRatio, 0.1f, 100.0f);
    }
}

void Camera::ToggleOrthographic(bool ortho)
{
    _orthographic = ortho;

    if (_orthographic)
    {
        _projectionMatrix = glm::ortho(0.0f , (float)_viewportHeight, 0.0f, (float)_viewportWidth, 0.01f, 100.0f);
    }
    else
    {
        float aspectRatio = ((float) _viewportWidth) / ((float) _viewportHeight);
        _projectionMatrix = glm::perspective(_fov, aspectRatio, 0.01f, 100.0f);
    }
}

void Camera::SetViewportSize(int x, int y)
{
    _viewportWidth = x;
    _viewportHeight = y;

    if (_orthographic)
    {
        _projectionMatrix = glm::ortho(0.0f, (float)_viewportHeight, 0.0f, (float)_viewportWidth, 0.01f, 100.0f);
    }
    else
    {
        float aspectRatio = ((float) _viewportWidth) / ((float) _viewportHeight);
        _projectionMatrix = glm::perspective(_fov, aspectRatio, 0.01f, 100.0f);
    }
}

void Camera::Rotate(const glm::vec3 & axis, float angle)
{
	_localRotation = glm::rotate(angle, axis);
// BEGIN_MARIANO: Para poder dirigir la camara con las flechas en el visor
//_target = glm::vec3(_localRotation * glm::vec4(_target,1));
// END_MARIANO.
    _viewMatrix = _localRotation * _viewMatrix;
}

void Camera::Translate(const glm::vec3 & amount)
{
	_viewMatrix = glm::translate(_viewMatrix, amount);
// BEGIN_MARIANO: Para poder dirigir la camara con las flechas en el visor
//_target += amount;
//_origin += amount;
// END_MARIANO.

}

const glm::vec3 & Camera::GetTarget() const
{
    return _target;
}

const glm::vec3 & Camera::GetOrigin() const
{
    return _origin;
}

const float Camera::GetFOV() const
{
    return _fov;
}

const bool Camera::IsOrthographic() const
{
    return _orthographic;
}

const glm::mat4 & Camera::GetProjectionMatrix() const
{
    return _projectionMatrix;
}

const glm::mat4 & Camera::GetViewMatrix() const
{
    return _viewMatrix;
}
