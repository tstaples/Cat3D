#ifndef INCLUDED_EDITORAPP_H
#define INCLUDED_EDITORAPP_H

#include <Engine.h>

#include "InputManager.h"
#include "InputData.h"
#include "EditorObject.h"
#include "InputCallbacks.h"

class Gizmo;

class EditorApp : public Application
{
public:
	EditorApp();
	virtual ~EditorApp();

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height) override;
	virtual void OnTerminate() override;
	virtual bool OnInput(const InputEvent& evt) override;
	virtual void OnUpdate() override;
    virtual void OnResizeWindow() override;

private:
    // Application
	Window mWindow;
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    // Screen
    u32 mWidth;
    u32 mHeight;

    // Input
    InputData mInputData;
    InputManager mInputManager;
    InputCallbacks mCallbacks;

    Gizmo* mpGizmo;

    // Game
    GameWorld mGameWorld;
    bool mIsGameRunning;

    Octree<EditorObject> mOctree;
    std::vector<EditorObject> mObjects;
    std::vector<EditorObject*> mSelectedObjects;

private:
    friend class InputCallbacks;
    friend class EditorCommands;
};

#endif //#ifndef INCLUDED_EDITORAPP_H