#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Engine.h>


class GameApp : public Application
{
public:
	GameApp();
	virtual ~GameApp();

private:
	// Application implementation
	virtual void OnInitialize(u32 width, u32 height);
	virtual void OnTerminate();
	virtual bool OnInput(const InputEvent& evt);
	virtual void OnUpdate();

private:
	Window mWindow;
	InputSystem mInputSystem;
	GraphicsSystem mGraphicsSystem;
	
    TextureManager mTextureManager;
    ModelManager mModelManager;

	Camera mCamera;
	Timer mTimer;
    Terrain mTerrain;
    
    GameObjectRepository mGameObjectRepo;
    TransformRepository mTransformRepo;
    ModelRepository mModelRepo;
    RenderService mRenderService;
    GameObjectFactory mGameObjectFactory;
    
    Math::Vector3 mBallPos;
};

#endif //#ifndef INCLUDED_GAMEAPP_H