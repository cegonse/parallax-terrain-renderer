// Terrain Viewer
//
// Windows application manager, file: WindowsApplication.cpp
//
// - WindowsApplication class is the manager of the application
//   for Windows environments. It encapsulates all the management
//   of the user interface handling and other low-level elements.
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


#include "WindowsApplication.h"
#include "ScreenshotWritter.h"

/* File-global instance of the class */
static WindowsApplication* application;

/* Window handler descriptors for all the dialogs and UI widgets in the application */
static HWND hwndParent, hwndRenderer, hwndDialog, hwndClose = 0, hwndHide = 0, hwndDialogLabel = 0, hwndLoadProject = 0, hwndCameraSlider = 0,
	hwndActualCameraLabel = 0, hwndDistanceToNextLabel = 0, hwndPlayButton = 0, hwndResetButton = 0, hwndMemory = 0, hwndWireframe = 0,
	hwndManageLod = 0, hwndLodValue = 0, hwndLodDistance = 0, hwndLodLabel = 0, hwndLodDistLabel = 0;

/* Switches for the behavior of the user interface */
static bool showControls = true, useWireframe = false, moveCamera = false;

static UINT memoryTimerLapse = 200;

/* Temporary variables for calculating mouse movement */
static int lastMouseX = 0, lastMouseY = 0;

/* Identifiers of the UI widgets */
enum
{
    IDC_QUIT_BUTTON = 101,
    IDC_HIDE_BUTTON,
    IDC_LOAD_BUTTON,
    IDC_CAMERA_SCROLL,
	IDC_PLAY_BUTTON,
	IDC_RESET_BUTTON,
	IDT_MEMORY_TIMER,
	IDC_WIREFRAME_BUTTON,
	IDC_MANAGELOD_BUTTON,
	IDC_MAXLOD_EDIT,
	IDC_MAXDIST_EDIT
};

/* Class constructor */
WindowsApplication::WindowsApplication(HINSTANCE hInstance, const std::string & title, int width, int height, WindowsApplicationCallbacks* callbacks)
{
	/* Save this instance of the class */
	application = this;

	/* This tells the application to listen to changes on the 
	   screen's pixel density (DPI). This disables the default
	   OS behavior of scaling all UI elements on non-aware
	   applications .
	*/
	SetProcessDPIAware();

	_runningTime = time(NULL);
    _instance = hInstance;
    _screenHeight = height;
    _screenWidth = width;
    _screenBpp = 32;
    _titleName = title;
    _activeProject = NULL;
	_renderer = NULL;
	_cameraTravel = NULL;
	_memoryTimerNow = time(NULL);
	_memoryTimerPrevious = time(NULL);

	/* Save application event callbacks */
	if (callbacks != NULL)
	{
		_renderEngineCreatedCallback = callbacks->renderEngineCreatedCallback;
		_projectOpenedCallback = callbacks->projectOpenedCallback;
	}
	else
	{
		_renderEngineCreatedCallback = NULL;
		_projectOpenedCallback = NULL;
	}

	_hglrc = 0;
	_hdc = 0;

	/*
	  The application dialog is composed by three windows:
	   - The main window acts as a canvas for the other
	     two windows.
	   - The rendering window holds the OpenGL context
	     and listens to mouse events for dragging or
		 clicking.
	   - The dialog window holds the different widgets of
	     the user interface.
	*/

	/* Set-up of the main window */
    _windowClass.cbSize        = sizeof(WNDCLASSEX);
    _windowClass.style         = 0;
    _windowClass.lpfnWndProc   = WindowProcedure;
    _windowClass.cbClsExtra    = 0;
    _windowClass.cbWndExtra    = 0;
    _windowClass.hInstance     = _instance;
    _windowClass.hIcon         = LoadIcon(_instance, IDI_APPLICATION);
    _windowClass.hIconSm       = 0;
    _windowClass.hCursor       = LoadCursor(0, IDC_ARROW);
    _windowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    _windowClass.lpszMenuName  = 0;
    _windowClass.lpszClassName = "_terrenosApplication";
    _windowClass.hIconSm       = LoadIcon(_instance, IDI_APPLICATION);

    if (!RegisterClassEx(&_windowClass))
    {
        throw std::exception();
    }

    _hwnd = CreateWindowEx(WS_EX_WINDOWEDGE,
        "_terrenosApplication", (LPCSTR) _titleName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth + 256 + 17, _screenHeight + 40,
        HWND_DESKTOP, NULL, _instance, NULL);

	/* Save the instance of the main window in the global variable */
	hwndParent = _hwnd;

    ShowWindow(_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(_hwnd);

	/* Rendering window set-up */
    _windowClass.cbSize        = sizeof(WNDCLASSEX);
    _windowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    _windowClass.lpfnWndProc   = WindowProcedure;
    _windowClass.cbClsExtra    = 0;
    _windowClass.cbWndExtra    = 0;
    _windowClass.hInstance     = _instance;
    _windowClass.hIcon         = LoadIcon(_instance, IDI_APPLICATION);
    _windowClass.hIconSm       = 0;
    _windowClass.hCursor       = LoadCursor(0, IDC_ARROW);
    _windowClass.hbrBackground = (HBRUSH) NULL;
    _windowClass.lpszMenuName  = 0;
    _windowClass.lpszClassName = "_terrenosApplicationRenderer";
    _windowClass.hIconSm       = LoadIcon(_instance, IDI_APPLICATION);

    if (!RegisterClassEx(&_windowClass))
    {
        throw std::exception();
    }

    _hwndRenderer = CreateWindowEx(WS_EX_WINDOWEDGE,
        "_terrenosApplicationRenderer", (LPCSTR) "Renderer", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight,
        _hwnd, NULL, _instance, NULL);

	/* Save the instance of the rendering window in the global variable */
	hwndRenderer = _hwndRenderer;

    ShowWindow(_hwndRenderer, SW_SHOWDEFAULT);
    UpdateWindow(_hwndRenderer);

	/* Dialog window set-up */
    _windowClass.cbSize        = sizeof(WNDCLASSEX);
    _windowClass.style         = 0;
    _windowClass.lpfnWndProc   = WindowProcedure;
    _windowClass.cbClsExtra    = 0;
    _windowClass.cbWndExtra    = 0;
    _windowClass.hInstance     = _instance;
    _windowClass.hIcon         = LoadIcon(_instance, IDI_APPLICATION);
    _windowClass.hIconSm       = 0;
    _windowClass.hCursor       = LoadCursor(0, IDC_ARROW);
    _windowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    _windowClass.lpszMenuName  = 0;
    _windowClass.lpszClassName = "_terrenosApplicationDialog";
    _windowClass.hIconSm       = LoadIcon(_instance, IDI_APPLICATION);

    if (!RegisterClassEx(&_windowClass))
    {
        throw std::exception();
    }

    _hwndDialog = CreateWindowEx(WS_EX_WINDOWEDGE,
        "_terrenosApplicationDialog", (LPCSTR) "Dialog", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        _screenWidth, 0, 256, _screenHeight,
        _hwnd, NULL, _instance, NULL);

	/* Save the instance of the dialog window in the global variable */
	hwndDialog = _hwndDialog;

    ShowWindow(_hwndDialog, SW_SHOWDEFAULT);
    UpdateWindow(_hwndDialog);
	
	/* Creation of the OpenGL context */
	try
	{
		CreateContext();
	}
	catch (const std::exception &)
	{
		MessageBox(GetActiveWindow(), GetLocalizedLastError().c_str(), "Error creating the OpenGL context", MB_OK | MB_ICONSTOP);
		exit(1);
	}

	/* The Windows API only exposes a limited set of basic UI widgets.
	   Loading the Windows Common Controls lets the application use
	   many other widgets that have been introduced after the early
	   versions of Windows. 
	*/
	INITCOMMONCONTROLSEX initCmctlEx;
	initCmctlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	initCmctlEx.dwICC = ICC_WIN95_CLASSES;

    InitCommonControlsEx(&initCmctlEx);

	_lastGetMessage = true;
	_wantToQuit = false;
}

WindowsApplication::~WindowsApplication()
{
	/* Release the memory used by the project data */
    DisposeActiveProject();

	/* Clean-up the OpenGL context */
    wglMakeCurrent(0, 0);
    wglDeleteContext(_hglrc);
    ReleaseDC(_hwndRenderer, _hdc);
}

Renderer* WindowsApplication::GetRenderEngine() const
{
    return _renderer;
}

/* Should the application run? */
bool WindowsApplication::Run()
{
    _keyUp = 0;
    _keyDown = 0;

	_memoryTimerNow = time(NULL);

	/* Update the used / available video memory every 2 seconds */
	if (difftime(_memoryTimerNow, _memoryTimerPrevious) > 2)
	{
		UpdateMemoryLabel();
		_memoryTimerPrevious = time(NULL);
	}

	/* _wantToQuit will be true when the user presses the
	   quit button or the application receives the quit
	   message from the operating system. */
    return !_wantToQuit;
}

/* Start a new frame */
void WindowsApplication::DoFrame(Color* clearColor) const
{
	/* Start time for profiling */
	uint64_t startTime = GetTickCount64();

	/* Check the message queue for any unattended operating
	   system messages. */
	if (PeekMessage((LPMSG) &_message, _hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_message);
		DispatchMessage(&_message);
	}

	/* The render engine maybe hasn't finished loading */
	if (_renderer != NULL)
	{
		/* Start a new frame on the renderer */
		_renderer->DoFrame(clearColor);

		/* If there is a loaded project, render it on the current frame */
		if (_activeProject != NULL)
		{
			_renderer->DrawProject(_activeProject, _manageLod, _maxDistance, _maxLod);
		}

		/* If there is an active camera travel, update its state */
		if (_cameraTravel != NULL)
		{
			_cameraTravel->DoFrame();

			/* Update of the UI labels */
			std::string labelActualCamera = "Current camera: ";
			labelActualCamera.append(boost::lexical_cast<std::string>(_cameraTravel->GetActualIndex()));
			SetWindowText(hwndActualCameraLabel, labelActualCamera.c_str());

			std::string labelDist = "Distance to the next cam.:\n";
			labelDist.append(boost::lexical_cast<std::string>(_cameraTravel->GetDistanceToNext()));
			SetWindowText(hwndDistanceToNextLabel, labelDist.c_str());
		}
	}

	/* Save the frame time if there is a loaded profiler */
	uint64_t endTime = GetTickCount64();
	Profiler* prof = _renderer->GetProfiler();

	if (prof != NULL)
	{
		prof->AddMainLoopTime(endTime - startTime);
	}
}

/* Commit the current frame */
void WindowsApplication::EndFrame() const
{
    SwapBuffers(_hdc);
}

/* Called when the application gets a key
   press message from the operating system. */
void WindowsApplication::OnKeyDown(WPARAM wParam)
{
    _keyDown = wParam;
}

/* Called when the application gets a key
   release message from the operating system. */
void WindowsApplication::OnKeyUp(WPARAM wParam)
{
    _keyUp = wParam;
}

/* Called when the application gets a mouse
   movement message from the operating system. */
void WindowsApplication::OnMouseMove(int x, int y)
{
    _mousePosition.x = (float)x;
    _mousePosition.y = (float)y;
}

/* Which is the last key that has been pressed? */
unsigned char WindowsApplication::GetKeyDown() const
{
    unsigned char result = 0;

    switch (_keyDown)
    {
    case VK_UP:
        result = KEY_UP_ARROW;
        break;

    case VK_DOWN:
        result = KEY_DOWN_ARROW;
        break;

    case VK_LEFT:
        result = KEY_LEFT_ARROW;
        break;

    case VK_RIGHT:
        result = KEY_RIGHT_ARROW;
        break;

	case VK_F2:
		result = KEY_F2;
		break;

	case VK_F3:
		result = KEY_F3;
		break;

	case 0x57:
		result = KEY_W;
		break;

	case 0x53:
		result = KEY_S;
		break;
	case 0x50:
		result = KEY_P;
		break;
    };

    return result;
}

/* What is the last stored mouse position? */
const glm::vec2 & WindowsApplication::GetMousePosition() const
{
    return _mousePosition;
}

void WindowsApplication::SetWindowTitle(const std::string & title)
{
	SetWindowText(_hwnd, title.c_str());
}

/* OpenGL context creation */
void WindowsApplication::CreateContext()
{
	/* Get the device context assigned to the render window  */
	HWND hwnd = _hwndRenderer;
	_hdc = GetDC(hwnd);

	if (_hdc == NULL)
	{
		throw std::exception();	
	}

	/* Set the objective pixel format for the context */
	if (!SetApplicationPixelFormat(_hdc, _screenBpp, 24, 8))
	{
		ReleaseDC(_hwndRenderer, _hdc);
		throw std::exception();
	}

	if (_hdc != 0)
	{
		/* The functoin wglCreateContext creates a compatibility
		   context, usually compatible up to OpenGL 3 on current
		   systems. To enable other OpenGL versions and to disable
		   backwards compatibility (the objective is using a core
		   profile) the function wglCreateContextAttribsARB is used.
		   
		   This function is an extension of OpenGL itself, so before
		   it can be used a compatibility OpenGL context must exist.
		   
		   The procedure to circumvent this is creating a temporary
		   context with wglCreateContext and then creating the
		   definitive context using wglCreateContextAttribsARB.
		*/
		HGLRC tempContext = wglCreateContext(_hdc);

		if (tempContext == NULL)
		{
			ReleaseDC(_hwndRenderer, _hdc);
			throw std::exception();
		}

		/* Set the temporary context as the current context */
		if (!wglMakeCurrent(_hdc, tempContext))
		{
			wglDeleteContext(tempContext);
			ReleaseDC(_hwndRenderer, _hdc);
			throw std::exception();
		}

		/* Enable GLEW for OpenGL extension management */
		glewExperimental = true;

		if (glewInit() != GLEW_OK)
		{
			MessageBox(GetActiveWindow(), "Error starting GLEW", "Error", MB_OK | MB_ICONERROR);
			throw std::exception();
		}

		/* Attributes for wglCreateContextAttribsARB */
		int attribList[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Major version
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, // Minor version
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, // Use a core profile
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Enable >forward< compatibility
			0
		};

		/* Create the definitive context */
		_hglrc = wglCreateContextAttribsARB(_hdc, 0, attribList);

		if (_hglrc == NULL)
		{
			wglDeleteContext(tempContext);
			ReleaseDC(_hwndRenderer, _hdc);
			throw std::exception();
		}

		/* Set the definitive context as the current context
		   and clean up the temporary context. */
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);

		if (!wglMakeCurrent(_hdc, _hglrc))
		{
			ReleaseDC(_hwndRenderer, _hdc);
			wglDeleteContext(_hglrc);
			throw std::exception();
		}

		/* Disable V-Sync */
		wglSwapIntervalEXT(0);

		ReleaseDC(_hwndRenderer, _hdc);

		/* Once the OpenGL context has been created successfully,
		   the render engine can be started up. */
		try
		{
			_renderer = new Renderer(_screenWidth, _screenHeight);
		}
		catch (const std::exception &)
		{
			MessageBox(_hwndRenderer, "The application couldn't be started.", "Fatal error", MB_OK | MB_ICONERROR);
			exit(1);
		}

		/* Send the application event for engine creation finished */
		application->OnRenderEngineCreated();
	}
}

int WindowsApplication::FindBestPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
    int currMode;                               // pixel format mode ID
    int bestMode = 0;                           // return value, best pixel format
    int currScore = 0;                          // points of current mode
    int bestScore = 0;                          // points of best candidate
    PIXELFORMATDESCRIPTOR pfd;

    // search the available formats for the best mode
    bestMode = 0;
    bestScore = 0;

    for (currMode = 1; DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0; ++currMode)
    {
        // ignore if cannot support opengl
        if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            continue;

        // ignore if cannot render into a window
        if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW))
            continue;

        // ignore if cannot support rgba mode
        if ((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE))
            continue;

        // ignore if not double buffer
        if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            continue;

        // try to find best candidate
        currScore = 0;

        // colour bits
        if (pfd.cColorBits >= colorBits) ++currScore;
        if (pfd.cColorBits == colorBits) ++currScore;

        // depth bits
        if (pfd.cDepthBits >= depthBits) ++currScore;
        if (pfd.cDepthBits == depthBits) ++currScore;

        // stencil bits
        if (pfd.cStencilBits >= stencilBits) ++currScore;
        if (pfd.cStencilBits == stencilBits) ++currScore;

        // alpha bits
        if (pfd.cAlphaBits > 0) ++currScore;

        // check if it is best mode so far
        if (currScore > bestScore)
        {
            bestScore = currScore;
            bestMode = currMode;
        }
    }

    return bestMode;
}

bool WindowsApplication::SetApplicationPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
	bool success = true;
    PIXELFORMATDESCRIPTOR pfd;

    // Find out the best matched pixel format
    int pixelFormat = FindBestPixelFormat(hdc, colorBits, depthBits, stencilBits);

    if (pixelFormat == 0)
	{
        success = false;
	}

    // set members of PIXELFORMATDESCRIPTOR with given mode ID
    DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

    // set the fixel format
    if(!SetPixelFormat(hdc, pixelFormat, &pfd))
	{
        success = false;
	}

    return success;
}

void WindowsApplication::UpdateMemoryLabel()
{
	std::string memStr = "Available VRAM:\n";

	int availableMem = _renderer->GetAvailableVideoMemory();

	if (availableMem == -1)
	{
		memStr += "??";
	}
	else
	{
		memStr += boost::lexical_cast<std::string>(availableMem);
	}
	
	memStr += " MB / ";

	int totalMem = _renderer->GetTotalVideoMemory();

	if (totalMem == -1)
	{
		memStr += "N/A";
	}
	else
	{
		memStr += boost::lexical_cast<std::string>(totalMem);
	}

	memStr += " MB";

	SetWindowText(hwndMemory, memStr.c_str());
}

void WindowsApplication::SetActiveProject(Project* project)
{
    _activeProject = project;
}

Project* WindowsApplication::GetActiveProject() const
{
    return _activeProject;
}

void WindowsApplication::DisposeActiveProject()
{
    if (_activeProject != NULL)
    {
        delete _activeProject;
		_activeProject = NULL;
    }
}

void WindowsApplication::SetOnProjectOpenedCallback(void (*callback)(WindowsApplication*))
{
    _projectOpenedCallback = callback;
}

void WindowsApplication::SetOnRenderEngineCreatedCallback(void (*callback)(WindowsApplication*))
{
	_renderEngineCreatedCallback = callback;
}

void WindowsApplication::OnProjectOpened()
{
    if (_projectOpenedCallback != NULL)
    {
        _projectOpenedCallback(this);
    }

	std::string labelCameraCount = "Camera count: ";
	labelCameraCount.append(boost::lexical_cast<std::string>(_activeProject->GetCameraList().size()));
	SetWindowText(hwndDialogLabel, labelCameraCount.c_str());

	std::string labelActualCamera = "Current camera: ";
	labelActualCamera.append(boost::lexical_cast<std::string>(0));
	SetWindowText(hwndActualCameraLabel, labelActualCamera.c_str());
}

void WindowsApplication::OnRenderEngineCreated()
{
	if (_renderEngineCreatedCallback != NULL)
	{
		int tm = (int) SetTimer(hwndRenderer, IDT_MEMORY_TIMER, memoryTimerLapse, (TIMERPROC) NULL);

		if (tm == 0)
		{
			MessageBox(0, GetLocalizedLastError().c_str(), "Error creating timer", MB_OK | MB_ICONSTOP);
		}

		UpdateMemoryLabel();

		_maxLod = _renderer->GetMaximumTessellationLevel();
		std::string maxTess = boost::lexical_cast<std::string>(_maxLod);
		SetWindowText(hwndLodValue, maxTess.c_str());

		_maxDistance = 30.0f;
		std::string maxDist = boost::lexical_cast<std::string>(_maxDistance);
		SetWindowText(hwndLodDistance, maxDist.c_str());

		_manageLod = false;

		_renderEngineCreatedCallback(this);
	}
}

void WindowsApplication::SetCameraTravel(CameraTravel* travel)
{
    _cameraTravel = travel;
}

CameraTravel* WindowsApplication::GetCameraTravel() const
{
	return _cameraTravel;
}

const time_t & WindowsApplication::GetRunningTime() const
{
	return _runningTime;
}

void WindowsApplication::SetLodManagement(bool manage)
{
	_manageLod = manage;
}

bool WindowsApplication::GetLodManagement() const
{
	return _manageLod;
}

void WindowsApplication::SetMaxLodLevel(int max)
{
	_maxLod = max;
}

int WindowsApplication::GetMaxLodLevel() const
{
	return _maxLod;
}

void WindowsApplication::SetMaxLodDistance(float dist)
{
	_maxDistance = dist;
}

float WindowsApplication::GetMaxLodDistance() const
{
	return _maxDistance;
}

void WindowsApplication::CommitQuit()
{
	_wantToQuit = true;
}

/* Window procedure for the application */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int windowType = 0;
    char className[128];

	/* Find which is the fetched message */
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

		case WM_CLOSE:
			{
				ProcessQuit();
			}
			return 0;
			break;

		/* The combination of the OpenGL and widget dialog on
		   the same window doesn't get renderer correctly unless
		   the paint message is done manually for some reason. */
		case WM_PAINT:
			{
				HDC hdc;
				PAINTSTRUCT ps;
				hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
			return 0;
			break;

        case WM_KEYDOWN:
            application->OnKeyDown(wParam);
            return 0;
            break;

        case WM_KEYUP:
            application->OnKeyUp(wParam);
            return 0;
            break;

        case WM_MOUSEMOVE:
            GetClassName(hwnd, className, sizeof(className));

			/* Only capture mouse movement on the renderer window */
            if (strcmp(className, "_terrenosApplicationRenderer") == 0)
            {
                int mouseX = LOWORD(lParam);
                int mouseY = HIWORD(lParam);

                application->OnMouseMove(mouseX, mouseY);

				/* Rotate the viewport */
				if (moveCamera)
				{
					Camera* cam = application->GetRenderEngine()->GetViewportCamera();
					glm::vec2 viewportSize = application->GetRenderEngine()->GetViewportSize();

					float hMove = 0.001f * (mouseX - lastMouseX);
					float vMove = 0.001f * (mouseY - lastMouseY);

					if (hMove > 89.0f)
					{
						hMove = 89.0f;
					}
					else if (hMove < -89.0f)
					{
						hMove = -89.0f;
					}
					
					float sinXrot = sin(glm::radians(hMove));
					float cosXrot = cos(glm::radians(hMove));

					float sinYrot = sin(glm::radians(vMove));
					float cosYrot = cos(glm::radians(vMove));

					glm::vec3 cameraTarget = cam->GetTarget();
					cam->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), hMove);
					cam->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), vMove);
				}

				lastMouseX = mouseX;
				lastMouseY = mouseY;
            }

            return 0;
            break;

		case WM_LBUTTONUP:
			GetClassName(hwnd, className, sizeof(className));

			/* Only capture mouse button actions on the renderer window */
			if (strcmp(className, "_terrenosApplicationRenderer") == 0)
			{
				if (moveCamera)
				{
					ShowCursor(true);
					moveCamera = false;
				}
			}

			return 0;
			break;

        case WM_LBUTTONDOWN:
            GetClassName(hwnd, className, sizeof(className));

			/* Only capture mouse button actions on the renderer window */
            if (strcmp(className, "_terrenosApplicationRenderer") == 0)
            {
                SetFocus(hwndRenderer);

				if (!moveCamera)
				{
					ShowCursor(false);
					moveCamera = true;
				}
            }

            return 0;
            break;

		case WM_HSCROLL:
			{
				int scroll = GetScrollPos(hwndCameraSlider, SB_CTL);
			}
			break;

        case WM_SIZE:
            GetClassName(hwnd, className, sizeof(className));

            if (strcmp(className, "_terrenosApplication") == 0)
            {
                windowType = WND_PARENT;
            }
            else if (strcmp(className, "_terrenosApplicationRenderer") == 0)
            {
                windowType = WND_RENDERER;
            }
            else if (strcmp(className, "_terrenosApplicationDialog") == 0)
            {
                windowType = WND_DIALOG;
            }

			/* Change the position of the different window elements upon resize */
            if (windowType == WND_PARENT)
            {
                int newWidth = LOWORD(lParam);
                int newHeight = HIWORD(lParam);

                if (application != NULL)
                {
                    if (application->GetRenderEngine() != NULL)
                    {
                        if (showControls)
                        {
							if (application->GetRenderEngine() != NULL)
							{
								application->GetRenderEngine()->SetViewportSize(newWidth - 256, newHeight);

								SetWindowPos(hwndRenderer, NULL, 0, 0, newWidth - 256, newHeight, SWP_NOZORDER);
								SetWindowPos(hwndDialog, NULL, newWidth - 256, 0, 256, newHeight, SWP_NOZORDER);

								if (hwndClose != 0)
								{
									SetWindowPos(hwndClose, NULL, 78, newHeight - 34, 100, 24, SWP_NOZORDER);
								}

								if (hwndHide != 0)
								{
									SetWindowPos(hwndHide, NULL, 78, newHeight - 74, 100, 24, SWP_NOZORDER);
								}

								UpdateWindow(hwndRenderer);
								UpdateWindow(hwndDialog);
							}
                        }
                        else
                        {
							if (application->GetRenderEngine() != NULL)
							{
								application->GetRenderEngine()->SetViewportSize(newWidth - 32, newHeight);

								SetWindowPos(hwndRenderer, NULL, 0, 0, newWidth - 32, newHeight, SWP_NOZORDER);
								SetWindowPos(hwndDialog, NULL, newWidth - 32, 0, 32, newHeight, SWP_NOZORDER);

								if (hwndHide != 0)
								{
									SetWindowPos(hwndHide, NULL, 6, newHeight / 2 - 50, 20, 100, SWP_NOZORDER);
								}

								UpdateWindow(hwndRenderer);
								UpdateWindow(hwndDialog);
							}
                        }
                    }
                }
            }

            return 0;
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
				/* Change the maximum level of detail */
				case IDC_MAXLOD_EDIT:
					if (HIWORD(wParam) == EN_CHANGE)
					{
						char editText[32];
						GetWindowText(hwndLodValue, editText, 32);
						
						try
						{
							int newLodLevel = boost::lexical_cast<int>(editText);
							
							/* Only change the level if its less than the maximum level
							   allowed by the hardware */
							if (application->GetRenderEngine()->GetMaximumTessellationLevel() < newLodLevel)
							{
								throw boost::bad_lexical_cast();
							}

							application->SetMaxLodLevel(newLodLevel);
						}
						catch (const boost::bad_lexical_cast &)
						{
							SetWindowText(hwndLodValue, boost::lexical_cast<std::string>(application->GetMaxLodLevel()).c_str());
						}
					}

					break;
				
				/* Change the maximum distance for  */
				case IDC_MAXDIST_EDIT:
				{
					if (HIWORD(wParam) == EN_CHANGE)
					{
						char editText[32];
						GetWindowText(hwndLodDistance, editText, 32);
						
						try
						{
							float newDistance = boost::lexical_cast<float>(editText);
							application->SetMaxLodDistance(newDistance);;
						}
						catch (const boost::bad_lexical_cast &)
						{
							SetWindowText(hwndLodDistance, boost::lexical_cast<std::string>(application->GetMaxLodDistance()).c_str());
						}
					}

					break;
				}

                case IDC_QUIT_BUTTON:
					{
						ProcessQuit();
					}
                    break;

                case IDC_HIDE_BUTTON:
                    {
                        showControls = !showControls;

                        RECT parentSize;
                        GetWindowRect(hwndParent, &parentSize);

                        int newWidth = parentSize.right - parentSize.left;
                        int newHeight = parentSize.bottom - parentSize.top;

						/* When the user presses the hide button, the application hides the dialog
						   containing the UI widgets. */
                        if (!showControls)
                        {
                            application->GetRenderEngine()->SetViewportSize(newWidth - 16, newHeight);

                            SetWindowPos(hwndRenderer, NULL, 0, 0, newWidth - 32 - 18, newHeight, SWP_NOZORDER);
                            SetWindowPos(hwndDialog, NULL, newWidth - 32 - 18, 0, 32, newHeight, SWP_NOZORDER);

                            if (hwndDialogLabel != 0)
                            {
                                ShowWindow(hwndDialogLabel, SW_HIDE);
                            }

                            if (hwndClose != 0)
                            {
                                ShowWindow(hwndClose, SW_HIDE);
                            }

                            if (hwndHide != 0)
                            {
                                SetWindowPos(hwndHide, NULL, 6, newHeight / 2 - 50, 20, 100, SWP_NOZORDER);
                                SetWindowText(hwndHide, "<");
                            }
                        }
                        else
                        {
                            application->GetRenderEngine()->SetViewportSize(newWidth - 256, newHeight);

                            SetWindowPos(hwndRenderer, NULL, 0, 0, newWidth - 256, newHeight, SWP_NOZORDER);
                            SetWindowPos(hwndDialog, NULL, newWidth - 256, 0, 256, newHeight, SWP_NOZORDER);

                            if (hwndDialogLabel != 0)
                            {
                                ShowWindow(hwndDialogLabel, SW_SHOW);
                            }

                            if (hwndClose != 0)
                            {
                                ShowWindow(hwndClose, SW_SHOW);
                            }

                            if (hwndHide != 0)
                            {
                                SetWindowPos(hwndHide, NULL, 78, newHeight - 124, 100, 24, SWP_NOZORDER);
                                SetWindowText(hwndHide, "Hide");
                            }

                            UpdateWindow(hwndRenderer);
                            UpdateWindow(hwndDialog);
                        }
                    }
                    break;

				/* Load a new project */
                case IDC_LOAD_BUTTON:
                    {
                        std::string fileName = "";
                        bool fileSelected = false;

						/* Display a file selection dialog to get the path to the project */
                        {
                            OpenFileDialog openFileDialog;

                            openFileDialog._filter = "XML Files (*.xml)\0*.xml\0";
                            openFileDialog._filterIndex = 0;
                            openFileDialog._owner = hwnd;
                            openFileDialog._title = "Select the project file";

                            if (openFileDialog.ShowDialog())
                            {
                                fileName.append(openFileDialog._fileName);
                                fileSelected = true;
                            }
                        }

                        if (fileSelected)
                        {
							/* Close and clean up the currently loaded project if 
							   there is one. */
                            if (application->GetActiveProject() != NULL)
                            {
                                application->DisposeActiveProject();
                            }
                            
							/*try
							{*/
								/* Try to load the selected project */
								application->SetActiveProject(MarshallProject(fileName, application->GetRenderEngine()));

								/* Send the application event for project loaded */
								application->OnProjectOpened();
							/*}
							catch (const std::exception &)
							{
								MessageBox(0, "The project couldn't be opened.", "Error", MB_OK | MB_ICONSTOP);
							}*/
                        }
                    }
                    break;
                case IDC_CAMERA_SCROLL:
                    {
                    }
                    break;

				/* Resume the camera travel on its waypoints */
				case IDC_PLAY_BUTTON:
					{
						if (application != NULL)
						{
							if (application->GetActiveProject() != NULL)
							{
								if (application->GetCameraTravel() != NULL)
								{
									if (application->GetCameraTravel()->IsPlaying())
									{
										application->GetCameraTravel()->Stop();
										SetWindowText(hwndPlayButton, ">");
									}
									else
									{
										application->GetCameraTravel()->Play();
										SetWindowText(hwndPlayButton, "||");
									}
								}
							}
						}
					}
					break;

					/* Reset the camera travel to the first waypoint */
					case IDC_RESET_BUTTON:
					{
						if (application != NULL)
						{
							if (application->GetActiveProject() != NULL)
							{
								if (application->GetCameraTravel() != NULL)
								{
									application->GetCameraTravel()->Reset();
								}
							}
						}
					}
					break;

					/* Switch from wireframe or fill polygon render modes */
					case IDC_WIREFRAME_BUTTON:
					{
						useWireframe = !useWireframe;

						if (application->GetRenderEngine() != NULL)
						{
							application->GetRenderEngine()->SetWireframe(useWireframe);
						}
					}
					break;

					/* Switch from fixed or automatic level of detail */
					case IDC_MANAGELOD_BUTTON:
					{
						application->SetLodManagement(!application->GetLodManagement());
					}
					break;
            }

            return 0;
            break;

        case WM_CREATE:
            GetClassName(hwnd, className, sizeof(className));

            if (strcmp(className, "_terrenosApplication") == 0)
            {
                windowType = WND_PARENT;
            }
            else if (strcmp(className, "_terrenosApplicationRenderer") == 0)
            {
                windowType = WND_RENDERER;
            }
            else if (strcmp(className, "_terrenosApplicationDialog") == 0)
            {
                windowType = WND_DIALOG;
            }

			/* Create all the dialog UI widgets */
            if (windowType == WND_DIALOG)
            {
                hwndClose = CreateWindowEx
                (
                    0,
                    "BUTTON",
                    "Quit",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    78, 556, 100, 24,
                    hwnd,
                    (HMENU)IDC_QUIT_BUTTON,
                    GetModuleHandle(NULL),
                    0
                );

                hwndLoadProject = CreateWindowEx
                (
                    0,
                    "BUTTON",
                    "Load Project",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    53, 30, 150, 24,
                    hwnd,
                    (HMENU)IDC_LOAD_BUTTON,
                    GetModuleHandle(NULL),
                    0
                );

                hwndHide = CreateWindowEx
                (
                    0,
                    "BUTTON",
                    "Hide",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    78, 516, 100, 24,
                    hwnd,
                    (HMENU)IDC_HIDE_BUTTON,
                    GetModuleHandle(NULL),
                    0
                );

                hwndDialogLabel = CreateWindow
                (
                    "STATIC",
                    "Camera count: ",
                    SS_LEFT | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                    10, 80, 200, 24,
                    hwnd,
                    0,
                    GetModuleHandle(NULL),
                    NULL
                 );

				hwndActualCameraLabel = CreateWindow
                (
                    "STATIC",
                    "Current camera: ",
                    SS_LEFT | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                    10, 109, 200, 24,
                    hwnd,
                    0,
                    GetModuleHandle(NULL),
                    NULL
                 );

				hwndDistanceToNextLabel = CreateWindow
                (
                    "STATIC",
                    "Distance to the next cam.:\n",
                    SS_LEFT | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                    10, 138, 200, 48,
                    hwnd,
                    0,
                    GetModuleHandle(NULL),
                    NULL
                 );

				hwndPlayButton = CreateWindowEx
				(
					NULL,
					"BUTTON",
					">",
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    78, 191, 100, 24,
                    hwnd,
                    (HMENU)IDC_PLAY_BUTTON,
                    GetModuleHandle(NULL),
                    0
                );

				hwndResetButton = CreateWindowEx
				(
					NULL,
					"BUTTON",
					"Reset",
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    78, 220, 100, 24,
                    hwnd,
                    (HMENU)IDC_RESET_BUTTON,
                    GetModuleHandle(NULL),
                    0
                );

				hwndMemory = CreateWindow
                (
                    "STATIC",
					"Available VRAM:\nN/A",
                    SS_LEFT | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                    10, 249, 200, 48,
                    hwnd,
                    0,
                    GetModuleHandle(NULL),
                    NULL
                 );

				hwndWireframe = CreateWindowEx
				(
					NULL,
					"BUTTON",
					"Wireframe",
					BS_AUTOCHECKBOX | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
					10, 302, 200, 30,
					hwnd,
					(HMENU) IDC_WIREFRAME_BUTTON,
					GetModuleHandle(NULL),
					0
				);

				hwndManageLod = CreateWindowEx
				(
					NULL,
					"BUTTON",
					"Automatic Level-of-Detail",
					BS_AUTOCHECKBOX | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
					10, 331, 200, 30,
					hwnd,
					(HMENU) IDC_MANAGELOD_BUTTON,
					GetModuleHandle(NULL),
					0
				);

				hwndLodLabel = CreateWindowEx
				(
					NULL,
					"STATIC",
					"Maximum level of detail",
					WS_VISIBLE | WS_CHILD,
					10, 365, 200, 24,
					hwnd,
					(HMENU) NULL,
					GetModuleHandle(NULL),
					0
				);

				hwndLodValue = CreateWindowEx
				(
					NULL,
					"EDIT",
					"64",
					WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | WS_TABSTOP,
					10, 389, 100, 24,
					hwnd,
					(HMENU) IDC_MAXLOD_EDIT,
					GetModuleHandle(NULL),
					0
				);

				hwndLodDistLabel = CreateWindowEx
				(
					NULL,
					"STATIC",
					"LOD minimum distance",
					WS_VISIBLE | WS_CHILD,
					10, 423, 200, 24,
					hwnd,
					(HMENU) NULL,
					GetModuleHandle(NULL),
					0
				);

				hwndLodDistance = CreateWindowEx
				(
					NULL,
					"EDIT",
					"30",
					WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | WS_TABSTOP,
					10, 447, 100, 24,
					hwnd,
					(HMENU) IDC_MAXDIST_EDIT,
					GetModuleHandle(NULL),
					0
				);
            }

            return 0;
            break;

        default:
           return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

std::string GetLocalizedLastError()
{
	std::string err = "";
	
	err = "Error: " + boost::lexical_cast<std::string>(GetLastError());

	return err;
}

void ProcessQuit()
{
	if (application != NULL)
	{
		Profiler* prof = application->GetRenderEngine()->GetProfiler();

		/* Save all the profiler statistics before quitting */
		if (prof != NULL)
		{
			time_t t = time(NULL);

			std::string date = ctime(&t);
			std::string hostData = "Windows x64 debug build";

			std::string execTime = boost::lexical_cast<std::string>(difftime(time(NULL), application->GetRunningTime()));
			execTime += " sec.";
			prof->SetExecutionTime(execTime);

			prof->WriteStatistics("statistics.txt", date, hostData);
		}

		application->CommitQuit();
	}
}