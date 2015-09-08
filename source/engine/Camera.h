// Terrain Viewer
//
// Camera, file: Camera.h
//
// - The Camera class implements a camera for both
//	 ortographic and perspective projections.
//
// - This class is an internal class and shouldn't be
//   instanced directly by user code. To instantiate a
//	 camera and obtain its reference properly, check
//   the CreateCamera method in the Renderer class.
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
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
    public:
		/*
		  Camera (constructor)

		  Input:
		   - origin (glm::vec3): position of the camera in world-space coordinates.
		   - target (glm::vec3): position of the camera target in world-space coordinates.
		   - fov (float): field-of-view of the camera lens in degrees.
		   - ortho (bool): set to true to create a camera with an orthographic projection.
		                   to false to create a camera with perspective projection.
		   - x (int): width of the render viewport.
		   - y (int): height of the render viewport.
		*/
        Camera(const glm::vec3 & origin, const glm::vec3 & target, float fov = 45.0f, bool ortho = false, int x = 0, int y = 0);
        
		/*
		  Camera (destructor)
		*/
		~Camera();

		/*
		  SetTarget,
		    sets the a new camera target.

		  Input:
		   - target (glm::vec3): position of the camera target in world-space coordinates.
		*/
        void SetTarget(const glm::vec3 & target);

		/*
		  SetOrigin,
		    sets a new camera position.

			Input:
			 - origin (glm::vec3): position of the camera in world-space coordinates.
		*/
        void SetOrigin(const glm::vec3 & origin);

		/* SetFOV,
		     sets a new field-of-view.
		   
		   Input:
		     - fov (float): the new field-of-view.
	    */
        void SetFOV(float fov);

		/*
		  ToggleOrthographic,
		    sets the projection to orthographic or perspective.
		  
		  Input:
		   - ortho (bool): orthographic projection if it is true,
						   perspective projection if it is false.
		*/
        void ToggleOrthographic(bool ortho);

		/*
		  SetViewportSize,
		    set a new render viewport size. This function is internal
			and shouldn't be called by user code. The renderer class
			takes care of updating all camera's viewport size when any
			kind of event changes it.
		  
		  Input:
		   - x (int): width of the viewport.
		   - y (int): height of the viewport.
		*/
        void SetViewportSize(int x, int y);

		/*
		  Rotate,
		    rotate the camera transform.

	      Input:
		   - axis (glm::vec3): axis in normalized coordinates for the rotation.
		   - angle (float): rotation angle in degrees.
		*/
        void Rotate(const glm::vec3 & axis, float angle);

		/*
		  Translate,
		    translate the camera transform.

		  Input:
		   - amount (glm::vec3): world-space vector with the translation.
		*/
		void Translate(const glm::vec3 & amount);

		/*
		  GetTarget,
		    returns the camera target.
		  
		  Output:
		   - (glm::vec3): position of the camera target in world-sapce coordinates.
		*/
        const glm::vec3 & GetTarget() const;

		/*
		  GetOrigin,
		    returns the camera position.
	      
		  Output:
		   - (glm::vec3): position of the camera in world-space coordinates.
		*/
        const glm::vec3 & GetOrigin() const;

		/*
		  GetFOV,
		    returns the camera field-of-view.

		  Output:
		   - (float): field-of-view of the camera in degrees.
		*/
        const float GetFOV() const;

		/*
		  IsOrthographic,
		    returns whether the camera is orthographic or not.
		  
		  Output:
		   - (bool): true if the camera has an orthographic projection,
		             false if the camera has a pespective projection.
		*/
        const bool IsOrthographic() const;

		/*
		  GetProjectionMatrix,
		    returns the camera transform projection matrix.
		  
		  Output:
		   - (glm::mat4): the camera projection matrix.
		*/
        const glm::mat4 & GetProjectionMatrix() const;

		/*
		  GetViewMatrix,
		    returns the camera transform view matrix.
		  
		  Output:
		   - (glm::mat4): the camera view matrix.
		*/
        const glm::mat4 & GetViewMatrix() const;

    private:
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;

        int _viewportWidth;
        int _viewportHeight;

        glm::vec3 _origin;
        glm::vec3 _target;
		glm::mat4 _localRotation;
        float _fov;
        bool _orthographic;
};

#endif // _CAMERA_H_
