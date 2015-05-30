#ifndef INCLUDED_TESTAPP_H
#define INCLUDED_TESTAPP_H

#include <Engine.h>

#include "InputManager.h"
#include "InputData.h"
#include "EditorObject.h"

class Gizmo;

class TestApp : public Application
{
public:
    typedef std::vector<EditorObject> EditorObjects;

	TestApp();
	virtual ~TestApp();
    
    const u8* GetSelectedObjectData(u32& size);
    void UpdateComponent(const u8* buffer, u32 buffsize);

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height) override;
	virtual void OnTerminate() override;
	virtual bool OnInput(const InputEvent& evt) override;
	virtual void OnUpdate() override;
    virtual void OnResizeWindow() override;

private:
	Window mWindow;
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    // Screen
    u32 mWidth;
    u32 mHeight;

    GameWorld mGameWorld;

    Octree<EditorObject> mOctree;
    EditorObjects mObjects;
    std::vector<EditorObject*> mSelectedObjects;
    
    Gizmo* mpGizmo;

    // Input
    InputData mInputData;
    InputManager mInputManager;

    bool mGameRunning;

private:

    // Input Actions
    bool OnSelectObject();
    bool OnZoom();

    // Input Axis events
    bool OnPanCamera(s32 val);
    bool OnCameraLook(s32 val);
    bool OnMouseDrag(s32 val);
};

#endif //#ifndef INCLUDED_TESTAPP_H