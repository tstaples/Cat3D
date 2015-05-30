#include "GameWorldApp.h"


int CALLBACK WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp testApp;
	testApp.Initialize(instance, "Test app", 1024, 768);
	while (testApp.IsRunning())
	{
		testApp.Update();
	}
	testApp.Terminate();
	return 0;
}