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

	// Eventually will be a separate tool
	void LoadModel(const char* filename, Model& model);

private:
	Window mWindow;
	InputSystem mInputSystem;
	GraphicsSystem mGraphicsSystem;
	
	Camera mCamera;
	Timer mTimer;
	
	MeshRenderer mRenderer;
	Model mModel;
    Texture mTexture;
};

#endif //#ifndef INCLUDED_TESTAPP_H