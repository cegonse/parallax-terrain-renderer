#include "Tester.h"
#include "WebHandler.h"

Tester::Tester(int width, int height, int minTl, int maxTl, int captureCount, int runCount, float dmax, const std::string & path, WebHandler* hndl)
{
	_run = true;
	_currentRun = 0;
	_lastFps = 0.0;
	_currentTl = minTl;

	_animationTimer = 0.0;
	_cpuFrequency = 0.0;
	_deltaTime = 0.0;
	_handler = hndl;
	_started = true;

	SetUp(width, height, minTl, maxTl, captureCount, runCount, dmax, path, hndl);
}

Tester::Tester()
{
	_run = true;
	_currentRun = 0;
	_lastFps = 0.0;
	_currentTl = _minTl;

	_animationTimer = 0.0;
	_cpuFrequency = 0.0;
	_deltaTime = 0.0;
	_started = false;
}


Tester::~Tester()
{
	delete _project;
	delete _renderer;
	glfwDestroyWindow(_glContext);
	glfwTerminate();
}


void Tester::SetUp(int width, int height, int minTl, int maxTl, int captureCount, int runCount, float dmax, const std::string & path, WebHandler* hndl)
{
	_width = width;
	_height = height;
	_minTl = minTl;
	_maxTl = maxTl;
	_captureCount = captureCount;
	_runCount = runCount;
	_dMax = dmax;
	_projectPath = path;

	_started = false;
	Init();
	SetAnimationData();
}


bool Tester::IsRunning() const
{
	return _started;
}


void Tester::Init()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		throw std::invalid_argument("Error loading GLFW");
	}

	// Create GLFW rendering context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_glContext = glfwCreateWindow(_width, _height, "SERVER RUNNING - DO NOT CLOSE", NULL, NULL);

	if (!_glContext)
	{
		glfwTerminate();
		throw std::invalid_argument("Error loading GLFW");
	}

	// Create OpenGL context
	glfwMakeContextCurrent(_glContext);
	glfwSwapInterval(0);

	// Initialize GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw std::invalid_argument("Error loading GLEW");
	}
	
	// Instantiate renderer
	try
	{
 		_renderer = new Renderer(_width, _height);
	}
	catch (std::invalid_argument &ex)
	{
		glfwTerminate();
		throw ex;
	}
	catch (std::exception &)
	{
		glfwTerminate();
		throw std::invalid_argument("Error creating renderer instance");
	}

	// Load project
	try
	{
		_project = MarshallProject(_projectPath, _renderer);
	}
	catch (std::exception &)
	{
		throw std::invalid_argument("Error loading terrain project.");
	}

	_runType = CURRENT_RUN_FPS;
}


void Tester::SetAnimationData()
{
	// Path data
	_waypoints = _project->GetCameraList();
	
	_curveLength = 0.0f;
	_distancePerCapture = 0.0f;
	_distancePerSample = 0.0f;
	_distanceToNext = 0.0f;

	_pathIndex = 1;
	_viewportCamera = _renderer->GetViewportCamera();

	for (unsigned int i = 0; i < _waypoints.size(); i += 3)
	{
		if (i + 3 < _waypoints.size())
        {
			/* Generate a cubic curve with the samples of waypoints */
			BezierCurve pathSpline;

			pathSpline.AddControlPoint(glm::vec2(_waypoints[i]->GetOrigin().x, _waypoints[i]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_waypoints[i + 1]->GetOrigin().x, _waypoints[i + 1]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_waypoints[i + 2]->GetOrigin().x, _waypoints[i + 2]->GetOrigin().z));
			pathSpline.AddControlPoint(glm::vec2(_waypoints[i + 3]->GetOrigin().x, _waypoints[i + 3]->GetOrigin().z));

			/* Evaluate the curve to obtain the final camera travel points */
			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				_pathPoints.push_back(glm::vec3(pathSpline.Evaluate(t).x, _project->GetConstantHeight(), pathSpline.Evaluate(t).y));
				_pathTangents.push_back(glm::vec3(pathSpline.Derive(t).x, 0, pathSpline.Derive(t).y));

				if (_pathPoints.size() >= 2)
				{
					_curveLength += glm::distance(_pathPoints[_pathPoints.size() - 2], _pathPoints[_pathPoints.size() - 1]);
				}
			}
		}
	}

	_distancePerCapture = _curveLength / static_cast<float>(_captureCount + 1);

	for (unsigned int i = 0; i < _captureCount; i++)
	{
		_captureDistance.push_back(i * _distancePerCapture + _distancePerCapture);
	}

	_distancePerSample = _curveLength / static_cast<float>(_pathPoints.size());

	_cameraPosition = _pathPoints[0];
	_cameraTarget = _pathPoints[0];

	_cameraTarget.x += _pathTangents[0].x;
	_cameraTarget.y -= 0.5;
	_cameraTarget.z += _pathTangents[0].y;

	_totalDistance = 0.0f;
	_capturesDone = 0;

	for (unsigned int i = 0; i < _runCount; i++)
	{
		_testData.push_back(TestData());

		_testData[i].meanFps.resize(_maxTl - _minTl + 2);
		_testData[i].triangles.resize(_maxTl - _minTl + 2);
	}

	_currentRunFps = 0.0;
	_currentRunFpsSamples = 0;
	_captureDist = 0.0f;
}


void Tester::Start(bool pollEvents)
{
	Color clearColor(1.0f, 1.0f, 1.0f);
	bool contextClose = false;
	bool nextGetTris = false;
	unsigned int tlIndex = 0;

	int cpr = _pathPoints.size() / (_captureCount + 1);
	int cpi = 0;

	while (_run && !contextClose)
	{
		if (pollEvents)
		{
			contextClose = glfwWindowShouldClose(_glContext) == 1;
		}
		
		// Camera animation
		//if (_pathIndex == 1)
		//{
		//	std::cout << "";
		//}

		_origin0 = _pathPoints[_pathIndex - 1];
		_target0 = glm::vec3(_pathPoints[_pathIndex - 1].x, _pathPoints[_pathIndex - 1].y - 0.5 * _pathPoints[_pathIndex].y, _pathPoints[_pathIndex - 1].z)  + _pathTangents[_pathIndex - 1];
		//_origin1 = _pathPoints[_pathIndex];
		//_target1 = glm::vec3(_pathPoints[_pathIndex].x, _pathPoints[_pathIndex].y - 0.5 * _pathPoints[_pathIndex].y, _pathPoints[_pathIndex].z) + _pathTangents[_pathIndex];

		_cameraPosition = _origin0;//glm::mix(_origin0, _origin1, _animationTimer);
		_cameraTarget = _target0;//glm::mix(_target0, _target1, _animationTimer);

		_viewportCamera->SetOrigin(_cameraPosition);
		_viewportCamera->SetTarget(_cameraTarget);

		// Start counting time
		QueryPerformanceCounter(&_startTime);

		// Render terrain
		_renderer->DoFrame(&clearColor);
		_renderer->DrawProject(_project, false, _dMax, _currentTl, nextGetTris);

		if (nextGetTris)
		{
			_testData[_currentRun].triangles[tlIndex].push_back(_renderer->GetLastFrameTriangles());
			nextGetTris = false;
		}

		glfwSwapBuffers(_glContext);

		// Get frame FPS and delta time
		// CPU Frequency should be fetched regularly, since the system
		// can change it depending on its load on mobility processors
		// and systems with technologies similar to Intel Turbo Boost
		QueryPerformanceCounter(&_endTime);
		QueryPerformanceFrequency(&_processorFrequency);
		_cpuFrequency = static_cast<double>(_processorFrequency.QuadPart);

		sprintf_s(_title, 128, "TL: %d, Run: %d, Run Type: %d, Td: %f / %f", _currentTl, _currentRun, _runType, _totalDistance, _distancePerCapture);
		glfwSetWindowTitle(_glContext, _title);

		_deltaTime = static_cast<double>((_endTime.QuadPart - _startTime.QuadPart) / _cpuFrequency);
		//_animationTimer += 0.1;//_deltaTime;
		//_captureDist += _deltaTime;
		_lastFps = 1.0 / _deltaTime;

		if (pollEvents)
		{
			glfwPollEvents();
		}

		// Obtain statistics and flow control
		//_distanceToNext = abs(glm::distance(_cameraPosition, _origin1));
		_currentRunFps += _lastFps;
		_currentRunFpsSamples++;

		if (cpi == cpr)
		{
			cpi = 0;

			if (_runType == CURRENT_RUN_TRIS)
			{
				nextGetTris = true;
			}
			else if (_runType == CURRENT_RUN_MSE)
			{
				if (_currentRun == 0)
				{
					MemoryTexture* tex = _renderer->GetFramebufferPixels();

					char fileName[128];
					sprintf_s(fileName, 128, "capture_%d_%d.tiff", _currentTl, _capturesDone);

					SaveScreenshot(tex, fileName);
					delete tex;
				}
			}
			else if (_runType == CURRENT_RUN_FPS)
			{
				_testData[_currentRun].meanFps[tlIndex].push_back(_currentRunFps / static_cast<double>(_currentRunFpsSamples));
				_currentRunFps = 0.0;
				_currentRunFpsSamples = 0;
			}

			_capturesDone++;
		}

		/*if (_distanceToNext < 0.01f)
		{
			_totalDistance += _distancePerSample;*/
		if (_pathIndex < _pathPoints.size() - 1)
		{
			_pathIndex++;
			cpi++;
			//_animationTimer = 0;
		}
		else
		{
			_pathIndex = 1;
			cpi = 0;
			_currentRun++;
			//_totalDistance = 0.0f;
			_capturesDone = 0;

			if (_currentRun == _runCount)
			{
				_currentTl++;
				tlIndex++;
				_currentRun = 0;

				if (_currentTl > _maxTl)
				{
					_currentTl = _minTl;
					tlIndex = 0;

					if (_runType == CURRENT_RUN_FPS)
					{
						_runType = CURRENT_RUN_TRIS;
					}
					else if (_runType == CURRENT_RUN_TRIS)
					{
						_runType = CURRENT_RUN_MSE;
					}
					else if (_runType == CURRENT_RUN_MSE)
					{
						_run = false;
					}
				}
			}
		}
		//}
	}

	WriteStatistics();
}


void Tester::WriteStatistics()
{
	// Save FPS
	for (unsigned int run = 0; run < _testData.size(); run++)
	{
		char path[128];
		sprintf_s(path, 128, "fps_%d_%d_%d.dlm", run, _minTl, _maxTl);
		std::ofstream file(path);

		for (int tl = 0; tl < _testData[run].meanFps.size(); tl++)
		{
			for (int cap = 0; cap < _testData[run].meanFps[tl].size(); cap++)
			{
				file << _testData[run].meanFps[tl][cap] << " ";
			}

			file << std::endl;
		}

		file.close();
	}

	// Save Tris
	for (unsigned int run = 0; run < _testData.size(); run++)
	{
		char path[128];
		sprintf_s(path, 128, "tris_%d_%d_%d.dlm", run, _minTl, _maxTl);
		std::ofstream file(path);

		for (int tl = 0; tl < _testData[run].triangles.size(); tl++)
		{
			for (int cap = 0; cap < _testData[run].triangles[tl].size(); cap++)
			{
				file << _testData[run].triangles[tl][cap] << " ";
			}

			file << std::endl;
		}

		file.close();
	}
}