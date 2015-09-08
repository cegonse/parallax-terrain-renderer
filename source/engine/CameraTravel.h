// Terrain Viewer
//
// Camera Travel, file: CameraTravel.h
//
// - The Camera travel class lets the user do a flight
//   over the terrain introducing arbitrary waypoints.
//
//   The class will interpolate these points using a
//   cubic curve to obtain smooth transitions between
//   points and to ensure that every instance of the
//   travel with the same waypoints will yield the same
//   result.
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
#ifndef _CAMERATRAVEL_H_
#define _CAMERATRAVEL_H_

#include <vector>
#include <windows.h>
#include <iostream>
#include <cmath>

#include "Camera.h"
#include "BuildSettings.hpp"

#include "UniformBSpline.h"
#include "BezierCurve.h"

class CameraTravel
{
    public:
		/*
		  CameraTravel (constructor)

		  Input:
		   - cameras(std::vector<Camera>): vector with all the cameras
		         acting as waypoints.
		   - renderCamera (Camera): a pointer to the current render
		         camera instance.
		   - timeStep (float): time step for the travel in seconds.
		   - sampleCount (int): how many samples of the cubic curve must
		          have been travelled in order to fire an index change event.
		*/
        CameraTravel(const std::vector<Camera*> & cameras, Camera* renderCamera, float timeStep = 20.0f, int sampleCount = 10);

		/*
		  DoFrame,
		    updates the state of the camera travel, should be called every
			time the time must advance.
		*/
        void DoFrame();

		/*
		  SetTimeStep,
		    sets a new time step in seconds.
		  
		  Input:
		   - t (float): the new time step in seconds.
		*/
        void SetTimeStep(float t);

		/*
		  GetTimeStep,
		    returns the current time step.
		  
		  Output:
		   - (float): current time step in seconds.
		*/
        float GetTimeStep() const;

		/*
		  SetTime,
		    changes the state of the timer.
		  
		  Input:
		   - time (float): the new state of the timer in seconds.
		*/
		void SetTime(float time);

		/*
		  GetTime,
		    returns the current timer state.
		  
		  Output:
		   - (float): current timer state in seconds.
		*/
		float GetTime() const;

		/*
		  Play,
		    starts the camera flight.
		*/
		void Play();

		/*
		  Stop,
		    pauses the camera flight.
		*/
		void Stop();

		/*
		  Reset,
		    restarts the camera flight to its initial state.
		*/
		void Reset();

		/*
		  IsPlaying,
		    returns whether the flight is playing or not.
		  
		  Output:
		   - (bool): true if it is playing, false if it is paused.
		*/
		bool IsPlaying() const;

		/*
		  GetActualIndex,
		    returns the current index of the camera travel.
		  
		  Output:
		   - (int): current index of the travel.
		*/
		int GetActualIndex() const;

		/*
		  GetDistanceToNext,
		    returns the distance to the next waypoint.
		  
		  Output:
		   - (float): distance to the next waypoint in world-space
					  units.
		*/
		float GetDistanceToNext() const;

		/*
		  SetOnCameraIndexChangedCallback,
		    sets the callback to be called when the camera index changes.
		  
		  Input:
		   - (void function pointer, void arguments): function pointer to the
		       callback.
		*/
		void SetOnCameraIndexChangedCallback(void (*callback)(void));

    private:
        std::vector<Camera*> _cameras;
        Camera* _renderCamera;

        float _timeStep;
        float _timer;
		float _mixTimer;
        int _actualIndex;
		bool _playing;
		float _distanceToNext;

		void UpdateCamera();

		float _nowTime;
		float _lastTime;

		void (*_cameraIndexChangedCallback)(void);

		std::vector<glm::vec3> _curvePoints;
		std::vector<glm::vec3> _curveDerivatives;

		float _sampleCount;
		float _sampleSpacing;
};

#endif // _CAMERATRAVEL_H_
