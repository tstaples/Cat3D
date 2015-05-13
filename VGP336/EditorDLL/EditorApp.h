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

    GameObjectPool mGameObjectPool;
    Octree<EditorObject> mOctree;

    std::vector<EditorObject> mObjects;
    std::vector<EditorObject*> mSelectedObjects;

    // Screen
    u32 mWidth;
    u32 mHeight;

    // Input
    InputData mInputData;
    InputManager mInputManager;

    u8 mObjBuffer[2048];

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