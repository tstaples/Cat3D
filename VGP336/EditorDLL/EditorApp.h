#ifndef INCLUDED_EDITORAPP_H
#define INCLUDED_EDITORAPP_H

#include <Engine.h>

class EditorApp : public Application
{
public:
	EditorApp();
	virtual ~EditorApp();

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height);
	virtual void OnTerminate();
	virtual bool OnInput(const InputEvent& evt);
	virtual void OnUpdate();

private:
	Timer mTimer;
	InputSystem mInputSystem;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    bool mKeyStates[256];
    s32 mMouseX;
    s32 mMouseY;
};

#endif //#ifndef INCLUDED_EDITORAPP_H