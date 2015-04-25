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
    //Math::Vector3 MouseToWorld();
    bool SelectedObjectInWorld(const Math::AABB& aabb);
    //Math::Ray GetMouseRay();

private:
	Window mWindow;
	Timer mTimer;
	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

    Octree mOctree;
    Math::AABB testAABB;

    // Screen
    u32 mWidth;
    u32 mHeight;

    // Input
    bool mKeyStates[256];
    bool mMouseStates[4];
    s32 mMouseX;
    s32 mMouseY;
    f32 mMouseMoveX;
    f32 mMouseMoveY;
    s8 mMouseScrollDelta;

    std::map<u32, Delegate0<void>> mActionMap;  // called when the corresponding input is fired
    std::map<bool*, Delegate0<void>> mAxisMap;    // Called every frame
    void OnMouseRightClick();
};

#endif //#ifndef INCLUDED_TESTAPP_H