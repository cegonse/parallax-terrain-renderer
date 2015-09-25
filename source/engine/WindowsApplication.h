// Terrain Viewer
//
// Windows application manager, file: WindowsApplication.h
//
// - Header for the WindowsApplication class.
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

#ifndef WINDOWSAPPLICATION_H
#define WINDOWSAPPLICATION_H

#include <windows.h>
#include <string>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>
#include <boost/scoped_array.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <commctrl.h>
#include <ctime>

#include "Renderer.h"
#include "WindowsOpenFileDialog.h"
#include "Project.h"
#include "CameraTravel.h"

#include "BuildSettings.hpp"

enum WindowType
{
    WND_PARENT = 0,
    WND_RENDERER,
    WND_DIALOG
};

enum KeyCode
{
    KEY_NONE = 0,
    KEY_UP_ARROW,
    KEY_DOWN_ARROW,
    KEY_LEFT_ARROW,
    KEY_RIGHT_ARROW,
	KEY_W,
	KEY_S,
	KEY_F2,
	KEY_F3,
	KEY_P
};

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void ProcessQuit();

struct WindowsApplicationCallbacks;

class WindowsApplication
{
    public:
        WindowsApplication(HINSTANCE hInstance, const std::string & title, int width, int height, WindowsApplicationCallbacks* callbacks = NULL);
        ~WindowsApplication();

        Renderer* GetRenderEngine() const;

        bool Run();
        void DoFrame(Color* clearColor) const;
        void EndFrame() const;

        void OnKeyDown(WPARAM wParam);
        void OnKeyUp(WPARAM wParam);
        void OnMouseMove(int x, int y);

        void SetActiveProject(Project* project);
        Project* GetActiveProject() const;
        void DisposeActiveProject();

        unsigned char GetKeyDown() const;
        unsigned char GetKeyUp() const;
        const glm::vec2 & GetMousePosition() const;

        void SetOnProjectOpenedCallback(void (*callback)(WindowsApplication*));
		void SetOnRenderEngineCreatedCallback(void (*callback)(WindowsApplication*));

        void OnProjectOpened();
		void OnRenderEngineCreated();

        void SetCameraTravel(CameraTravel* travel);
		CameraTravel* GetCameraTravel() const;

		void CreateContext();
		const time_t & GetRunningTime() const;

		void SetLodManagement(bool manage);
		bool GetLodManagement() const;

		void SetMaxLodLevel(int max);
		int GetMaxLodLevel() const;

		void SetMaxLodDistance(float dist);
		float GetMaxLodDistance() const;

		void SetWindowTitle(const std::string & title);

		void CommitQuit();

    private:
        int FindBestPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
        bool SetApplicationPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
		void UpdateMemoryLabel();

        HWND _hwnd, _hwndRenderer, _hwndDialog;
        WNDCLASSEX _windowClass;
        HINSTANCE _instance;
        MSG _message;
        HDC _hdc;
        HGLRC _hglrc;

        WPARAM _keyUp;
        WPARAM _keyDown;
        glm::vec2 _mousePosition;

        int _majorVersion;
        int _minorVersion;

        int _screenHeight;
        int _screenWidth;
        int _screenBpp;
        std::string _titleName;

        Project* _activeProject;
        Renderer* _renderer;
        CameraTravel* _cameraTravel;

        void (*_projectOpenedCallback)(WindowsApplication*);
		void (*_renderEngineCreatedCallback)(WindowsApplication*);

		time_t _memoryTimerNow;
		time_t _memoryTimerPrevious;
		time_t _runningTime;

		bool _manageLod;
		float _maxDistance;
		int _maxLod;
		bool _lastGetMessage;
		bool _wantToQuit;
};

struct WindowsApplicationCallbacks
{
	void (*projectOpenedCallback)(WindowsApplication*);
	void (*renderEngineCreatedCallback)(WindowsApplication*);
};

std::string GetLocalizedLastError();

#endif // WINDOWSAPPLICATION_H
