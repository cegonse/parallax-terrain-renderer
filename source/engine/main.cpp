// Terrain Viewer
//
// Main, file: main.cpp
//
// - This file is the main entry point for the application.
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

#include "Application.h"
#include "Color.h"
#include "Debug.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "WindowsApplication.h"
#include "Renderer.h"
#include "Terrain.h"
#include "ScreenshotWritter.h"

/* Define this macro when building on Visual Studio */
#define WINDOWS_BUILD

/* Callbacks for application events */
void OnProjectOpened(WindowsApplication* application);
void OnRenderEngineCreated(WindowsApplication* application);
void OnCameraTravelIndexChanged();

glm::vec3 cameraPosition(0.5f, 0.5f, -0.5f);
glm::vec3 target(0.0f, 0.0f, 0.0f);

/* Instance of the render engine */
Renderer* rendererInstance;

/* Instance of the application handler */
WindowsApplication* applicationInstance;

/* Instance of the viewport camera */
Camera* mainCameraInstance;

/* Main stub */
#ifdef WINDOWS_BUILD
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdArgs, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
	/* Used for appending a counter to the end of
	   saved screenshots. */
	int screenshotCount = 0;

	/* Colors used for the background clearing and
       for the axis lines. */
	Color background(1.0f, 1.0f, 1.0f);
    Color white(1.0f, 1.0f, 1.0f, 1.0f);
    Color red(1.0f, 0.0f, 0.0f, 1.0f);
    Color green(0.0f, 1.0f, 0.0f, 1.0f);
    Color blue(0.0f, 0.0f, 1.0f, 1.0f);

	/* Set debug to true to enable the user to
	   move the camera using the arrow keys and
	   to see the world axes. */
    bool debug = true;
	bool parallax = false;

	/* Callback container for the application events */
	WindowsApplicationCallbacks callbacks;
	callbacks.projectOpenedCallback = OnProjectOpened;
	callbacks.renderEngineCreatedCallback = OnRenderEngineCreated;

	/* Create instance of the application */
    WindowsApplication application(hInstance, "Terrain Viewer (Parallax OFF)", 800, 600, &callbacks);
	applicationInstance = &application;

	while (application.Run())
    {
		/* Start a new frame */
		application.DoFrame(&background);

        if (debug)
        {
			/* Movement with arrow keys */
            if (application.GetKeyDown() == KEY_LEFT_ARROW)
            {
                cameraPosition.x -= 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
            else if (application.GetKeyDown() == KEY_RIGHT_ARROW)
            {
                cameraPosition.x += 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
            else if (application.GetKeyDown() == KEY_UP_ARROW)
            {
                cameraPosition.z += 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
            else if (application.GetKeyDown() == KEY_DOWN_ARROW)
            {
                cameraPosition.z -= 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
			else if (application.GetKeyDown() == KEY_W)
			{
				cameraPosition.y += 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);	
			}
			else if (application.GetKeyDown() == KEY_S)
			{
				cameraPosition.y -= 0.005f;
                mainCameraInstance->SetOrigin(cameraPosition);
			}
			else if (application.GetKeyDown() == KEY_P)
			{
				parallax = !parallax;
				application.SetWindowTitle(parallax ? "Terrain Viewer (Parallax ON)" : "Terrain Viewer (Parallax OFF)");
				rendererInstance->ToggleParallax();
			}

			/* Drawing of world axes */
			if (rendererInstance != NULL)
			{
				//rendererInstance->Draw3DLine(glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), &red);
				//rendererInstance->Draw3DLine(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f), &green);
				//rendererInstance->Draw3DLine(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 100.0f), &blue);
			}
        }

		/* Save an screenshot whenever the user presses F2 */
		if (application.GetKeyDown() == KEY_F2)
		{
			boost::scoped_ptr<MemoryTexture> memtex(rendererInstance->GetFramebufferPixels());
			std::string path = "Screenshot_" + boost::lexical_cast<std::string>(screenshotCount++) + ".tiff";

			try
			{
				SaveScreenshot(memtex.get(), path);
			}
			catch (const std::exception &)
			{
				MessageBox(GetActiveWindow(), "Couldn't save the screenshot", "Error", MB_OK | MB_ICONSTOP);
			}
		}
		else if (application.GetKeyDown() == KEY_F3)
		{
			glm::vec3 camPos = mainCameraInstance->GetOrigin();
			char msg[128];
			sprintf_s(msg, 128, "(%f | %f | %f)", camPos.x, camPos.y, camPos.z);
			MessageBox(0, msg, "", MB_OK);
		}

		/* Commit the current frame */
        application.EndFrame();
    }

    return EXIT_SUCCESS;
}

/* This callback defines the behavior of the application
   when a project finishes loading. */
void OnProjectOpened(WindowsApplication* application)
{
	/* Get the list of waypoints in the project and an instance
	   of the current viewport camera. */
    Project* project = application->GetActiveProject();
    std::vector<Camera*> cameras = project->GetCameraList();
    Camera* renderCamera = application->GetRenderEngine()->GetViewportCamera();

	/* Create a camera travel with the waypoints in the project
	   and assign it to the application */
	CameraTravel* travel = new CameraTravel(cameras, renderCamera, 25.0f, project->GetProjectResolution());
	travel->SetOnCameraIndexChangedCallback(OnCameraTravelIndexChanged);
    application->SetCameraTravel(travel);
}

/* This callback defines the behavior of the application
   when the render engine finishes loading. */
void OnRenderEngineCreated(WindowsApplication* application)
{
	/* Set the global instance of the render engine */
	Renderer* renderer = application->GetRenderEngine();
	rendererInstance = renderer;

	/* Initial set-up of the render engine */
    Camera* mainCamera = renderer->CreateCamera(cameraPosition, target);
	mainCameraInstance = mainCamera;
    renderer->SetViewportCamera(mainCamera);
    renderer->SetWireframe(false);
}

/* This callback defines the behavior of the application when
   the camera travel changes its waypoint index. */
void OnCameraTravelIndexChanged()
{
	/* Take an screenshot every time the index changes */
	boost::scoped_ptr<MemoryTexture> memtex(rendererInstance->GetFramebufferPixels());
	std::string path = "Screenshot_Travel_" + boost::lexical_cast<std::string>(applicationInstance->GetCameraTravel()->GetActualIndex() - 1) + ".tiff";

	try
	{
		SaveScreenshot(memtex.get(), path);
	}
	catch (const std::exception &)
	{
		MessageBox(GetActiveWindow(), "Couldn't save the screenshot.", "Error", MB_OK | MB_ICONSTOP);
	}
}
