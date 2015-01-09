#ifndef INCLUDED_TESTAPP_H
#define INCLUDED_TESTAPP_H

#include <Engine.h>

class TestApp : public Application
{
public:
	TestApp();
	virtual ~TestApp();

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height);
	virtual void OnTerminate();
	virtual bool OnInput(const InputEvent& evt);
	virtual void OnUpdate();

private:
	Window mWindow;
	Timer mTimer;
	InputSystem mInputSystem;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;
};

#endif //#ifndef INCLUDED_TESTAPP_H