#include "WebHandler.h"


static WebHandler* hndlInstance = NULL;


WebHandler::WebHandler(const char* cmdArgs)
{
	hndlInstance = this;
	_tester = NULL;

	// Obtaining command line parameters
	// If there are any cmd line parameters,
	// run as local application. Input parameters
	// separated by semicolons
	//
	// 1: viewport width
	// 2: viewport height
	// 3: min tessellation level
	// 4: max tessellation level
	// 5: capture count
	// 6: run count
	// 7: max distance
	// 8: project path
	//
	// Example params:
	// 1024;768;1;64;1;3;10;C:\Users\Cesar\Documents\TFG\GIS\PUGET DEFINITIVO\EXPORT\project.xml
	_width = 800;
	_height = 600;
	_minTl = 1;
	_maxTl = 64;
	_captureCount = 1;
	_runCount = 1;
	_dmax = 5.0f;
	_projectPath = "C:\\Users\\Cesar\\Documents\\TFG\\GIS\\PUGET DEFINITIVO\\EXPORT\\project.xml";
	
	std::vector<std::string> cmd;
	SplitArguments(cmdArgs, cmd);

	if (cmd.size() == 8)
	{
		_width = boost::lexical_cast<int>(cmd[0]);
		_height = boost::lexical_cast<int>(cmd[1]);
		_minTl = boost::lexical_cast<int>(cmd[2]);
		_maxTl = boost::lexical_cast<int>(cmd[3]);
		_captureCount = boost::lexical_cast<int>(cmd[4]);
		_runCount = boost::lexical_cast<int>(cmd[5]);
		_dmax = boost::lexical_cast<float>(cmd[6]);
		_projectPath = cmd[7];

		_runLocal = true;
	}
	else
	{
		_runLocal = false;
	}
}


WebHandler::~WebHandler()
{
	if (_tester != NULL)
	{
		delete _tester;
	}

	if (!_runLocal)
	{
		mg_destroy_server(&_server);
	}
}


void WebHandler::Start(unsigned int port)
{
	// Create web server instance
	if (_runLocal)
	{
		StartTest();
	}
	else
	{
		_server = mg_create_server(NULL, OnServerEvent);
		mg_set_option(_server, "listening_port", boost::lexical_cast<std::string>(port).c_str());

		while (true)
		{
			mg_poll_server(_server, 1000);
		}
	}
}


bool WebHandler::StartTest()
{
	bool busy = false;

	if (!_runLocal)
	{
		if (!_tester->IsRunning())
		{
			/*_tester->SetUp(_width, _height, _minTl, _maxTl, _captureCount, _runCount, _dmax, _projectPath, this);
			_testThread = boost::thread(boost::ref(_tester), _runLocal);*/

			busy = true;
		}
	}
	else
	{
		_tester = new Tester(_width, _height, _minTl, _maxTl, _captureCount, _runCount, _dmax, _projectPath, this);
		_tester->Start(_runLocal);
	}
	
	return busy;
}



int OnServerEvent(mg_connection* conn, mg_event ev)
{
	int reqResult = MG_FALSE;

	switch (ev)
	{
		case MG_AUTH:
			reqResult = MG_TRUE;
			break;

		case MG_REQUEST:
			// Grab URI sent by the client
			std::string uri = conn->uri;

			reqResult = MG_TRUE;
			break;
	}

	if (hndlInstance != NULL)
	{
		if (hndlInstance->StartTest())
		{
			mg_printf_data(conn, "busy");
		}
		else
		{
			mg_printf_data(conn, "started");
		}
	}
	else
	{
		mg_printf_data(conn, "error");
	}

	return reqResult;
}



void WebHandler::SplitArguments(const char* cmd, std::vector<std::string> & v)
{
	std::string c = cmd;

	boost::char_separator<char> sep(";");
	boost::tokenizer<boost::char_separator<char>> t(c, sep);

	for (boost::tokenizer<boost::char_separator<char>>::iterator it = t.begin(); it != t.end(); it++)
	{
		v.push_back(*it);
	}
}

