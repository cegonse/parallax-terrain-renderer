#ifndef _WEBHANDLER_H_
#define _WEBHANDLER_H_

#include <boost/tokenizer.hpp>
#include <boost/cast.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <process.h>

#include "Tester.h"
#include "mongoose.h"


class WebHandler
{
	public:
		WebHandler(const char* cmdArgs);
		~WebHandler();
		void Start(unsigned int port);
		bool StartTest();

	private:
		void SplitArguments(const char* cmd, std::vector<std::string> & v);

		unsigned int _width;
		unsigned int _height;
		unsigned int _minTl;
		unsigned int _maxTl;
		unsigned int _captureCount;
		unsigned int _runCount;
		float _dmax;
		std::string _projectPath;
		bool _runLocal;

		mg_server* _server;
		boost::thread _testThread;
		Tester* _tester;
};

int OnServerEvent(mg_connection* conn, mg_event ev);
DWORD WINAPI RenderThread(__in LPVOID lpParameter);

#endif