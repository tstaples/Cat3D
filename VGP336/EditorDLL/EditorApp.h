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

    // Input
    InputData mInputData;
    InputManager mInputManager;
    InputCallbacks mCallbacks;

    Gizmo* mpGizmo;

    // Game
    GameWorld mGameWorld;
    bool mIsGameRunning;

    Octree<EditorObject> mOctree;
    EditorObjects mObjects;
    std::vector<EditorObject*> mSelectedObjects;

    Delegate<void, f32> UpdateDelegate;
    void OnEditorUpdate(f32 deltaTime);
    void OnGameUpdate(f32 deltaTime);

private:
    friend class InputCallbacks;
    friend class EditorCommands;
};

#endif //#ifndef INCLUDED_EDITORAPP_H