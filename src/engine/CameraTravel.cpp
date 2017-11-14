// Terrain Viewer
//
// Camera travel, file: CameraTravel.cpp
//
// - Implementation of the CameraTravel class.
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

#include "CameraTravel.h"

CameraTravel::CameraTravel(const std::vector<Camera*> & cameras, Camera* renderCamera, float timeStep, int sampleCount)
{
    _cameras = cameras;
    _renderCamera = renderCamera;
    _timeStep = timeStep;
    _timer = 0.0f;
    _actualIndex = 1;
	_sampleCount = 0.0f;
	_playing = false;
	_distanceToNext = 0;
	_mixTimer = 0;
	_cameraIndexChangedCallback = NULL;

	float curveLength = 0.0f;

	/* For all the waypoints in the project */
	for (int i = 0; i < _cameras.size(); i += 3)
	{
		if (i + 3 < _cameras.size())
        {
			/* Generate a cubic curve with the samples of waypoints */
			BezierCurve pathSpline;

			pathSpline.AddControlPoint(glm::vec2(_cameras[i]->GetOrigin().x, _cameras[i]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_cameras[i + 1]->GetOrigin().x, _cameras[i + 1]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_cameras[i + 2]->GetOrigin().x, _cameras[i + 2]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_cameras[i + 3]->GetOrigin().x, _cameras[i + 3]->GetOrigin().z));

			/* Evaluate the curve to obtain the final camera travel points */
			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				_curvePoints.push_back(glm::vec3(pathSpline.Evaluate(t).x, _cameras[i]->GetOrigin().y, pathSpline.Evaluate(t).y));
				_curveDerivatives.push_back(glm::vec3(pathSpline.Derive(t).x, 0, pathSpline.Derive(t).y));

				if (_curvePoints.size() >= 2)
				{
					curveLength += glm::distance(_curvePoints[_curvePoints.size() - 2], _curvePoints[_curvePoints.size() - 1]);
				}
			}
		}
	}

	/* The index changed event will be called every time the viewport
	   camera travels _sampleSpacing units. */
	_sampleSpacing = curveLength / sampleCount;
}

void CameraTravel::DoFrame()
{
	float dt = _nowTime - _lastTime;

	/* Only update data if enough time has passed since last update */
    if (dt >= _timeStep)
    {
        if (_playing)
		{
			if (_curvePoints.size() > 1)
			{
				_sampleCount +=  _timeStep / 1000.0f;
				UpdateCamera();
			}

			_timer += _timeStep / 1000.0f;
			_mixTimer += _timeStep / 1000.0f;
		}

		_lastTime = _nowTime;
    }
}

void CameraTravel::Reset()
{
	_actualIndex = 1;
	_mixTimer = 0.0f;
	_timer = 0.0f;
}

void CameraTravel::SetOnCameraIndexChangedCallback(void (*callback)(void))
{
	_cameraIndexChangedCallback = callback;
}

void CameraTravel::UpdateCamera()
{
	/* Set origin and destination points */
	glm::vec3 origin0 = _curvePoints[_actualIndex - 1];
    glm::vec3 target0 = glm::vec3(_curvePoints[_actualIndex - 1].x, _curvePoints[_actualIndex - 1].y - 0.5 * _curvePoints[_actualIndex].y, _curvePoints[_actualIndex - 1].z)  + _curveDerivatives[_actualIndex - 1];
    glm::vec3 origin1 = _curvePoints[_actualIndex];
    glm::vec3 target1 = glm::vec3(_curvePoints[_actualIndex].x, _curvePoints[_actualIndex].y - 0.5 * _curvePoints[_actualIndex].y, _curvePoints[_actualIndex].z) + _curveDerivatives[_actualIndex];

	/* Interpolate the current point */
    glm::vec3 origin = glm::mix(origin0, origin1, _mixTimer);
    glm::vec3 target = glm::mix(target0, target1, _mixTimer);

	/* Update the viewport camera */
    _renderCamera->SetOrigin(origin);
    _renderCamera->SetTarget(target);

	_distanceToNext = abs(glm::distance(_renderCamera->GetOrigin(), origin1));

	if (_sampleCount >= _sampleSpacing)
	{
		/* Fire an index changed event if the camera has travelled
		   the set distance. */
		if (_cameraIndexChangedCallback != NULL)
		{
			_cameraIndexChangedCallback();
		}

		_sampleCount = 0.0f;
	}

	if (_distanceToNext < 0.01f)
    {
		if (_actualIndex < _curvePoints.size() - 1)
		{
			_actualIndex++;
			_mixTimer = 0;
		}
		else
		{
			_playing = false;
		}
    }
}

float CameraTravel::GetDistanceToNext() const
{
	return _distanceToNext;
}

int CameraTravel::GetActualIndex() const
{
	return _actualIndex;
}

void CameraTravel::SetTimeStep(float t)
{
    _timeStep = t;
}

float CameraTravel::GetTimeStep() const
{
    return _timeStep;
}

void CameraTravel::Play()
{
	_playing = true;
}

void CameraTravel::Stop()
{
	_playing = false;
}

bool CameraTravel::IsPlaying() const
{
	return _playing;
}

void CameraTravel::SetTime(float time)
{
	_timer = time;
	UpdateCamera();
}

float CameraTravel::GetTime() const
{
	return _timer;
}
