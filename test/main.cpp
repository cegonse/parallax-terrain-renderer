#include "Tester.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdArgs, int nCmdShow)
{
	int width = 800;
	int height = 600;
	int minTl = 1;
	int maxTl = 64;
	int captureCount = 1;
	int runCount = 1;
	float dmax = 5.0f;
	std::string projectPath = "C:\\Users\\mariano\\Documents\\articulos\\Journal Supercomputing 2016\\tfg_cesar\\EXPORT\\project.xml";
	int parallaxType = PARALLAX_TYPE_NONE;
	
	std::vector<std::string> cmd;
	SplitArguments(lpCmdArgs, cmd);

	if (cmd.size() == 9)
	{
		width = boost::lexical_cast<int>(cmd[0]);
		height = boost::lexical_cast<int>(cmd[1]);
		minTl = boost::lexical_cast<int>(cmd[2]);
		maxTl = boost::lexical_cast<int>(cmd[3]);
		captureCount = boost::lexical_cast<int>(cmd[4]);
		runCount = boost::lexical_cast<int>(cmd[5]);
		dmax = boost::lexical_cast<float>(cmd[6]);
		projectPath = cmd[7];
		parallaxType = boost::lexical_cast<int>(cmd[8]);
	}

	try
	{
		Tester test(width, height, minTl, maxTl, captureCount, runCount, dmax, projectPath, parallaxType);
		test.Start(true);
	}
	catch (std::invalid_argument &ex)
	{
		MessageBox(0, ex.what(), "Error running server", MB_OK | MB_ICONSTOP);
	}
	catch (std::exception &)
	{
		MessageBox(0, "Error running server", "", MB_OK | MB_ICONSTOP);
	}

	return EXIT_SUCCESS;
}

void SplitArguments(const char* cmd, std::vector<std::string> & v)
{
	std::string c = cmd;

	boost::char_separator<char> sep(";");
	boost::tokenizer<boost::char_separator<char>> t(c, sep);

	for (boost::tokenizer<boost::char_separator<char>>::iterator it = t.begin(); it != t.end(); it++)
	{
		v.push_back(*it);
	}
}

