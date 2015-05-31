#ifndef INCLUDED_EDITORAPP_H
#define INCLUDED_EDITORAPP_H

#include <Engine.h>

#include "InputManager.h"
#include "InputData.h"
#include "EditorObject.h"
#include "InputCallbacks.h"

class Gizmo;

typedef std::vector<EditorObject> EditorObjects;

class EditorApp : public Application
{
public:
	EditorApp();
	virtual ~EditorApp();

    void InitializeGameView(HINSTANCE instance, HWND hWnd, LPCSTR appName, u32 width, u32 height);

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height) override;
	virtual void OnTerminate() override;
	virtual bool OnInput(const InputEvent& evt) override;
	virtual void OnUpdate() override;
    virtual void OnResizeWindow(HWND window) override;

private:
    // Application
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    // Input
    InputData mInputData;
    InputManager mInputManager;
    InputCallbacks mCallbacks;

    Gizmo* mpGizmo;

    // Game
	Window mGameWindow;
    GameWorld mGameWorld;
    bool mIsGameRunning;

    Octree<EditorObject> mOctree;
    EditorObjects mObjects;
    std::vector<EditorObject*> mSelectedObjects;

private:
    friend class InputCallbacks;
    friend class EditorCommands;
};

#endif //#ifndef INCLUDED_EDITORAPP_H