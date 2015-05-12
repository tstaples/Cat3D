#ifndef INCLUDED_TESTAPP_H
#define INCLUDED_TESTAPP_H

#include <Engine.h>

#include "InputManager.h"
#include "InputData.h"
#include "EditorObject.h"

class TestApp : public Application
{
public:
	TestApp();
	virtual ~TestApp();
    
    const u8* GetSelectedObjectData(u32& size);

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height);
	virtual void OnTerminate();
	virtual bool OnInput(const InputEvent& evt);
	virtual void OnUpdate();

private:
	Window mWindow;
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    // Screen
    u32 mWidth;
    u32 mHeight;

    GameObjectPool mGameObjectPool;
    Octree<EditorObject> mOctree;
    std::vector<EditorObject> mObjects;
    std::vector<EditorObject*> mSelectedObjects;
    
    u8 mObjBuffer[2048];

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
};

#endif //#ifndef INCLUDED_TESTAPP_H