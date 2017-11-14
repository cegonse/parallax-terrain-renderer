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

#include "Color.h"
#include "Debug.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "WindowsApplication.h"
#include "Renderer.h"
#include "Terrain.h"
#include "ScreenshotWritter.h"
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

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

//  Número de frames
int frameCount = 0;
//  Número de frames por segundo
float fps = 0;
//  currentTime y previousTime - momento actual y momento previo
clock_t currentTime = 0, previousTime = 0;
void calculateFPS(WindowsApplication &, bool);
float pixelsDiferentes(MemoryTexture *, MemoryTexture *);
float mse(MemoryTexture *, MemoryTexture *);

MemoryTexture *refScreenShot = NULL, *cmpScreenShot = NULL;
GLuint query[2];

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
	bool parallax = true;

	/* Callback container for the application events */
	WindowsApplicationCallbacks callbacks;
	callbacks.projectOpenedCallback = OnProjectOpened;
	callbacks.renderEngineCreatedCallback = OnRenderEngineCreated;

	/* Create instance of the application */
    WindowsApplication application(hInstance, "Terrain Viewer (Parallax ON)", 800, 600, &callbacks);
	applicationInstance = &application;

	glGenQueries(2, query);

	while (application.Run())
    {
		/* Start a new frame */
		glBeginQuery(GL_PRIMITIVES_GENERATED, query[0]);
		glBeginQuery(GL_TIME_ELAPSED, query[1]);
		application.DoFrame(&background);
		glEndQuery(GL_TIME_ELAPSED);
		glEndQuery(GL_PRIMITIVES_GENERATED);

		calculateFPS(application, parallax);

        if (debug)
        {
			/* Movement with arrow keys */
            if (application.GetKeyDown() == KEY_LEFT_ARROW)
            {
                cameraPosition.x -= 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
            else if (application.GetKeyDown() == KEY_RIGHT_ARROW)
            {
                cameraPosition.x += 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);
            }
            else if (application.GetKeyDown() == KEY_UP_ARROW)
            {
                cameraPosition.z += 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);
// BEGIN_MARIANO: Para poder dirigir la camara con las flechas en el visor
//				glm::vec3 target = mainCameraInstance->GetTarget();
//				cameraPosition += (target - cameraPosition) * 0.01f;
//				target += (target - cameraPosition) * 0.01f;
//                mainCameraInstance->SetOrigin(cameraPosition);
//                mainCameraInstance->SetTarget(target);
// END_MARIANO.
            }
            else if (application.GetKeyDown() == KEY_DOWN_ARROW)
            {
                cameraPosition.z -= 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);
// BEGIN_MARIANO: Para poder dirigir la camara con las flechas en el visor
//				glm::vec3 target = mainCameraInstance->GetTarget();
//				cameraPosition -= (target - cameraPosition) * 0.01f;
//				target -= (target - cameraPosition) * 0.01f;
//              mainCameraInstance->SetOrigin(cameraPosition);
//              mainCameraInstance->SetTarget(target);
// END_MARIANO.
            }
			else if (application.GetKeyDown() == KEY_W)
			{
				cameraPosition.y += 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);	
			}
			else if (application.GetKeyDown() == KEY_S)
			{
				cameraPosition.y -= 0.02f;
                mainCameraInstance->SetOrigin(cameraPosition);
			}
			else if (application.GetKeyDown() == KEY_P)
			{
				parallax = !parallax;
				application.SetWindowTitle(parallax ? "Terrain Viewer (Parallax ON)" : "Terrain Viewer (Parallax OFF)");
				rendererInstance->ToggleParallax();
			}
			else if (application.GetKeyDown() == KEY_F3 )
			{
				// Para depuracion
				//MessageBox(0, "And text here", "MessageBox caption", MB_OK);
			}
			/* Drawing of world axes */
			if (rendererInstance != NULL)
			{
				//rendererInstance->Draw3DLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), &red);
				//rendererInstance->Draw3DLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f), &green);
				//rendererInstance->Draw3DLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 100.0f), &blue);
			}
        }

		/* Save an screenshot whenever the user presses F2 */
		if (application.GetKeyDown() == KEY_F2)
		{
			if ( refScreenShot != NULL )
				delete refScreenShot;
			refScreenShot = rendererInstance->GetFramebufferPixels();
			std::string path = "Screenshot_" + boost::lexical_cast<std::string>(screenshotCount++) + ".tiff";

			try
			{
				char msg[128];
				sprintf_s(msg, 128, "Capturada imagen de referencia de %d x %d pixels\nGuardada en %s", refScreenShot->GetWidth(), refScreenShot->GetHeight(), path.c_str());
				SaveScreenshot(refScreenShot, path);
				MessageBox(0, msg, "", MB_OK);
			}
			catch (const std::exception &)
			{
				MessageBox(GetActiveWindow(), "Couldn't save the screenshot", "Error", MB_OK | MB_ICONSTOP);
			}
		}
		else if (application.GetKeyDown() == KEY_F3)
		{
			if ( cmpScreenShot != NULL )
				delete cmpScreenShot;
			cmpScreenShot = rendererInstance->GetFramebufferPixels();
			if ( refScreenShot == NULL )
			{
				MessageBox(0, "Imagen de referencia no establecida", "", MB_OK);
			}
			else if ( cmpScreenShot->GetWidth() != refScreenShot->GetWidth() || cmpScreenShot->GetHeight() != refScreenShot->GetHeight() )
			{
				char msg[128];
				sprintf_s(msg, 128, "El tamaño de la pantalla actual (%d x %d) no coincide con la de referencia(%d x %d)", cmpScreenShot->GetWidth(), cmpScreenShot->GetHeight() , refScreenShot->GetWidth(), refScreenShot->GetHeight());
				MessageBox(0, msg, "", MB_OK);
			}
			else
			{
				float _mse = mse(refScreenShot, cmpScreenShot);
				float _rmse = sqrt(_mse);
				float _psnr = 20 * log10(255.0) - 10 * log10(_mse);
				char msg[1024];
				sprintf_s(msg, 1024, "Comparando imagen actual con la de referencia\nPorcentaje pixels diferentes: %.2f\nMSE: %f\nRMSE: %f\nPSNR: %f", pixelsDiferentes(refScreenShot, cmpScreenShot), _mse, _rmse, _psnr);
				MessageBox(0, msg, "", MB_OK);				
			}
		}

		/* Commit the current frame */
        application.EndFrame();
    }

	if ( refScreenShot != NULL )
		delete refScreenShot;
	if ( cmpScreenShot != NULL )
		delete cmpScreenShot;
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

//-------------------------------------------------------------------------
// Calcula el frame rate por segundo
//-------------------------------------------------------------------------
void calculateFPS(WindowsApplication &app, bool parallax)
{
	frameCount++;

	static char msg[128];
	static string s;
	static GLuint64 timeElapsed = 0, aux;

	app.SetWindowTitle(s.c_str());

    currentTime = clock();
	glGetQueryObjectui64v(query[1], GL_QUERY_RESULT, &aux);
	timeElapsed += aux;

    long timeInterval = (currentTime - previousTime) / CLOCKS_PER_SEC;

    if(timeInterval > 1)
    {
        fps = float(frameCount) / timeInterval;
		float FPS = float(frameCount) / (timeElapsed / 1000000000.0);

		// wait until the results are available
		GLint stopObjectAvailable = 0;
		while (!stopObjectAvailable) {
			glGetQueryObjectiv(query[0], GL_QUERY_RESULT_AVAILABLE, &stopObjectAvailable);
		}

		GLint ntris;
		glGetQueryObjectiv(query[0], GL_QUERY_RESULT, &ntris);

        previousTime = currentTime;

		s = parallax ? "Terrain Viewer (-Parallax ON)" : "Terrain Viewer (-Parallax OFF)";
		sprintf_s(msg, 128, ". tris: %d, fps: %.2f, FPS: %.2f", ntris, fps, FPS);
		s += msg;

		frameCount = 0;
		timeElapsed = 0;
    }

}

//-------------------------------------------------------------------------
// Compara dos imagenes y obtiene el porcentaje de pixels diferentes (descartando los del fondo blanco)
// Se supone que img1 es la imagen de referencia y es de mayor resolución
//------------------------------------------------------------------------- 
float pixelsDiferentes(MemoryTexture * img1, MemoryTexture *img2)
{
	float res = 0.0f;

	if ( img1->GetHeight() == img2->GetHeight() && img1->GetWidth() == img2->GetWidth() )
	{
		unsigned long long count = 0, size = 0;
		int w = img1->GetWidth(), h = img1->GetHeight();
		GLubyte * data1 = img1->Get(), * data2 = img2->Get();
		for ( int j = 0; j < h; j ++ )
			for ( int i = 0; i < w; i ++ )
			{
				if ( data1[(j * w + i) * 3 + 0] == 255 && data1[(j * w + i) * 3 + 1] == 255 && data1[(j * w + i) * 3 + 2] == 255 )
					 continue;
				size ++;
				if (data1[(j * w + i) * 3 + 0] != data2[(j * w + i) * 3 + 0] || data1[(j * w + i) * 3 + 1] != data2[(j * w + i) * 3 + 1] || data1[(j * w + i) * 3 + 2] != data2[(j * w + i) * 3 + 2])
					count ++;
			}
		if ( size != 0 )
			res = 100.0f * float(count) / float(size);
	}

	return res;
}

//-------------------------------------------------------------------------
// Obtiene el MSE de dos imagenes (descartando los pixels del fondo blanco)
// Se supone que img1 es la imagen de referencia y es de mayor resolución
//------------------------------------------------------------------------- 
float mse(MemoryTexture * img1, MemoryTexture *img2)
{
	float res = 0.0f;

	if ( img1->GetHeight() == img2->GetHeight() && img1->GetWidth() == img2->GetWidth() )
	{
		unsigned long long count = 0, size = 0;
		int w = img1->GetWidth(), h = img1->GetHeight();
		GLubyte * data1 = img1->Get(), * data2 = img2->Get();
		float difr, difg, difb;
		for ( int j = 0; j < h; j ++ )
			for ( int i = 0; i < w; i ++ )
			{
				if ( data1[(j * w + i) * 3 + 0] == 255 && data1[(j * w + i) * 3 + 1] == 255 && data1[(j * w + i) * 3 + 2] == 255 )
					 continue;
				size ++;
				difr = data1[(j * w + i) * 3 + 0] - data2[(j * w + i) * 3 + 0];
				difg = data1[(j * w + i) * 3 + 1] - data2[(j * w + i) * 3 + 1];
				difb = data1[(j * w + i) * 3 + 2] - data2[(j * w + i) * 3 + 2];
				res += (difr * difr + difg * difg + difb * difb) / 3.0f;
			}
		if ( size != 0 )
			res /= float(size);
	}

	return res;
}