#ifndef INCLUDED_EDITORAPP_H
#define INCLUDED_EDITORAPP_H

#include <Engine.h>

#include "InputManager.h"
#include "InputData.h"
#include "EditorObject.h"

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
    //Math::Vector3 MouseToWorld();
    bool SelectedObjectInWorld(const Math::AABB& aabb);

    void UpdateControls(f32 deltaTime);

private:
	Window mWindow;
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    std::vector<EditorObject> mObjects;
    std::vector<EditorObject*> mSelectedObjects;
    Octree<EditorObject> mOctree;

    // Screen
    u32 mWidth;
    u32 mHeight;

    // Input
    InputData mInputData;
    InputManager mInputManager;

private:
    // Input Actions
    bool OnSelectObject();
    bool OnZoom();

    // Input Axis events
    bool OnPanCamera(s32 val);
    bool OnCameraLook(s32 val);
    bool OnCamerWalkForward(s32 val);
};

#endif //#ifndef INCLUDED_EDITORAPP_H