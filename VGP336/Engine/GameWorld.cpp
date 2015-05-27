//====================================================================================================
// Filename:	GameWorld.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "GameWorld.h"

#include "SerialReader.h"

//====================================================================================================
// local Definitions
//====================================================================================================

GameWorld::GameWorld(u16 maxObjects)
    : mGameObjectPool(maxObjects)
    , mFactory(mGameObjectPool)
{
}

//----------------------------------------------------------------------------------------------------

GameWorld::~GameWorld()
{
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnInitialize(const GameSettings& settings, GraphicsSystem& gs, Camera& camera)
{
    mSettings = settings;

    // Init services
    mRenderService.Initialize(gs, camera);

    // Store all our services in a list to pass to the GameObjectFactory
    Services services =
    {
        &mRenderService
    };
    mFactory.Initialize(services);
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnShutdown()
{
    mRenderService.Terminate();
    mFactory.Terminate();
    mGameObjectHandles.clear();
    mGameObjectPool.Flush();
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnInput()
{
    // TODO: Invoke script "OnInput" function
    return true;
}

//----------------------------------------------------------------------------------------------------

void GameWorld::OnUpdate(f32 deltaTime)
{
    // Update all the game objects
    for (GameObjectHandle& handle : mGameObjectHandles)
    {
        GameObject* gameObject = handle.Get();
        gameObject->Update(deltaTime);
    }

    // TODO: Update other services
}

//----------------------------------------------------------------------------------------------------

void GameWorld::OnRender()
{
    mRenderService.Update();
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameWorld::CreateGameObject(const char* templateFile, Math::Vector3 pos, Math::Quaternion rot)
{
    // Use the templateFile if it's not null, otherwise use the default object template.
    // Note: For now we're assuming all GameObjects have a transform component
    const char* templatePath = (templateFile) ? templateFile : "../Data/GameObjects/default.json";
    GameObjectHandle handle = mFactory.Create(templatePath);

    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    if (gameObject->GetComponent(transformComponent))
    {
        transformComponent->Translate(pos);
        // TODO: transformComponent->Rotate(rot);
    }

    // Add the object to the world
    mGameObjectHandles.push_back(handle);
    return handle;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::LoadLevel(const char* levelName)
{
    // Clear current level data
    mGameObjectHandles.clear();
    mGameObjectPool.Flush();
    mRenderService.UnSubscribeAll();

    // 1 level = 1 file for now
    // level loader will give us a vector of Level objects containing the buffers for each gameobject and the game settings
    // we then loop through each and call mFactory.Create(buffer) to make the object
    // then we delete the level buffers. TODO: have levelloader hold a managed pool that levels are loaded into.
    // If the level fails to load, crash?

    Level level;
    if (mLevelLoader.Load(levelName, level))
    {
        mSettings = level.settings;
        for (u32 i=0; i < level.numGameObjects; ++i)
        {
            SerialReader reader(level.buffer, level.bufferSize);
            GameObjectHandle handle = mFactory.Create(reader);
            mGameObjectHandles.push_back(handle);
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::SaveLevel(const char* levelName)
{
    return mLevelLoader.SaveToFile(levelName, mGameObjectHandles, mSettings);
}

/*
    Editor::OnPlay()
    - save current scene and store away
    - set isPlaying true
        GameWorld::OnPlay()
        - load startup scene (from settings)
        - add physics and scripting services to update list

    Editor::Update()
        GameWorld::Update()
        - update scene
            - update gameobjects
        - update services in service list
    - if (IsPlaying)


    GameWorld::OnPauseBegin()
    - remove physics and script services from update list


*/