


#include "WebHandler.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdArgs, int nCmdShow)
{
	try
	{
		WebHandler hndl(lpCmdArgs);
		hndl.Start(8088);
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



