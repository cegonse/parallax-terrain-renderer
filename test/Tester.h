#ifndef _TESTER_H_
#define _TESTER_H_

#include <iostream>
#include <Windows.h>
#include <boost/scoped_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include <fstream>


#include "Renderer.h"
#include "Project.h"
#include "BezierCurve.h"
#include "ScreenshotWritter.h"


#include <GLFW/glfw3.h>

enum CurrentRun
{
	CURRENT_RUN_FPS = 0,
	CURRENT_RUN_TRIS,
	CURRENT_RUN_MSE
};

struct TestData
{
	std::vector< std::vector<double> > meanFps;
	std::vector< std::vector<int> > triangles;
};

class WebHandler;

class Tester
{
	public:
		Tester(int width, int height, int minTl, int maxTl, int captureCount, int runCount, float dmax,
				const std::string & path, WebHandler* hndl);
		Tester();
		~Tester();

		void SetUp(int width, int height, int minTl, int maxTl, int captureCount, int runCount, float dmax,
				const std::string & path, WebHandler* hndl);
		void Start(bool pollEvents);
		bool IsRunning() const;
		void WriteStatistics();
	
	private:
		void SetAnimationData();
		void Init();

		bool _started;

		unsigned int _width;
		unsigned int _height;
		unsigned int _minTl;
		unsigned int _maxTl;
		unsigned int _captureCount;
		unsigned int _runCount;
		float _dMax;
		std::string _projectPath;

		bool _run;
		GLFWwindow* _glContext;
		Renderer* _renderer;
		Project* _project;
		char _title[128];

		unsigned int _currentRun;
		unsigned int _currentTl;
		double _lastFps;
		unsigned int _runType;

		LARGE_INTEGER _startTime;
		LARGE_INTEGER _endTime;
		LARGE_INTEGER _processorFrequency;

		double _deltaTime;
		double _cpuFrequency;
		double _animationTimer;
		double _currentRunFps;
		int _currentRunFpsSamples;
		std::vector<float> _captureDistance;

		std::vector<Camera*> _waypoints;
		std::vector<glm::vec3> _pathPoints, _pathTangents;
		float _curveLength;
		float _distancePerCapture;
		float _distancePerSample;
		float _distanceToNext;
		unsigned int _pathIndex;
		Camera* _viewportCamera;

		glm::vec3 _cameraPosition;
		glm::vec3 _cameraTarget;
		glm::vec3 _lastPosition;

		glm::vec3 _origin0, _origin1;
		glm::vec3 _target0, _target1;
		float _totalDistance;
		int _capturesDone;
		float _captureDist;

		std::vector<TestData> _testData;
		WebHandler* _handler;
};

#endif /* _TESTER_H_ */