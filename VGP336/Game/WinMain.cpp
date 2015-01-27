#include "GameApp.h"


int CALLBACK WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp gameApp;
	gameApp.Initialize(instance, "Game App", 1024, 768);
	while (gameApp.IsRunning())
	{
		gameApp.Update();
	}
	gameApp.Terminate();
	return 0;
}