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
	TestApp();
	virtual ~TestApp();
    
    const u8* GetSelectedObjectData(u32& size);
    void UpdateComponent(const u8* buffer, u32 buffsize);

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
    
    Gizmo* mpGizmo;

    u8 mObjBuffer[2048];

    // Input
    InputData mInputData;
    InputManager mInputManager;

    MeshBuffer mBuffer;
    MeshRenderer mRenderer;
    Mesh mCube;

private:
    // Input Actions
    bool OnSelectObject();
    bool OnZoom();

    // Input Axis events
    bool OnPanCamera(s32 val, s32 test, bool test2) { return false; }
    bool OnPanCamera(s32 val);
    bool OnCameraLook(s32 val);
    bool OnMouseDrag(s32 val);
};

#endif //#ifndef INCLUDED_TESTAPP_H