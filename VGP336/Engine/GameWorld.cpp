//====================================================================================================
// Filename:	GameWorld.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "GameWorld.h"

#include "Application.h"
#include "SerialReader.h"

namespace
{
    f32 kTimer = 0.0f;

    void DestroyGameObjectList(GameObjectHandles& handles, GameObjectFactory& factory)
    {
        const u32 size = handles.size();
        for (u32 i=size; i > 0; --i)
        {
            factory.Destroy(handles[i - 1]);
        }
        handles.clear();
    }
}

//====================================================================================================
// local Definitions
//====================================================================================================

GameWorld::GameWorld(Application* app, u16 maxObjects)
    : mpApplication(app)
    , mGameObjectPool(maxObjects)
    , mFactory(mGameObjectPool)
{
    mCurrentLevel.buffer = nullptr;
    mCurrentLevel.bufferSize = 0;
    mCurrentLevel.numGameObjects = 0;
}

//----------------------------------------------------------------------------------------------------

GameWorld::~GameWorld()
{
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnInitialize(const GameSettings& settings, GraphicsSystem& gs, Camera& camera)
{
    mpGraphicsSystem = &gs;
    mSettings = settings;

    mAssetLoader.Initialize(gs);
    mTextureManager.Initialize(gs);

    Math::AABB worldRegion(Math::Vector3::Zero(), Math::Vector3(100.0f, 100.0f, 100.0f));

    // Init services
    Service::spGameWorld = this; // Set the world
    mRenderService.Initialize(gs, camera, mAssetLoader);
    mPhysicsService.Initialize(worldRegion, 10);
    mTerrainService.Initialize(gs, camera, mTextureManager);

    // Store all our services in a list to pass to the GameObjectFactory
    mServiceList.push_back(&mRenderService);
    mServiceList.push_back(&mPhysicsService);
    mServiceList.push_back(&mTerrainService);

    mFactory.Initialize(mServiceList, *this);
    mFactory.OnDestroyGameObject = DELEGATE(&GameWorld::OnGameObjectDestroyed, this);
    
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnShutdown()
{
    // Unsubscribe and terminate services
    for (Service* service : mServiceList)
    {
        service->Terminate();
    }

    // Manually destroy all game objects to ensure resources are freed properly
    DestroyGameObjectList(mUpdateList, mFactory);
    DestroyGameObjectList(mDestroyedList, mFactory);

    mTextureManager.Terminate();
    mAssetLoader.Terminate();
    mFactory.Terminate();
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
    // Recompute size each time to account for new objects that may be added
    for (u32 i=0; i < mUpdateList.size(); ++i)
    {
        GameObjectHandle handle = mUpdateList[i];
        GameObject* gameObject = handle.Get();
        if (gameObject->IsValid())
        {
            gameObject->Update(deltaTime);
        }
    }

    // Prune the destroyed objects
    for (GameObjectHandle& handle : mDestroyedList)
    {
        // Terminate the gameObject

        // Find the handle in the update list
        const u32 numUpdate = mUpdateList.size();
        for (u32 i=0; i < numUpdate; ++i)
        {
            if (handle == mUpdateList[i])
            {
                // Remove from the update list
                mUpdateList[i] = mUpdateList.back();
                mUpdateList.pop_back();
                
                // Destroy the object
                mFactory.Destroy(handle);
                break;
            }
        }
    }
    mDestroyedList.clear();

    //kTimer += deltaTime;
    //if (kTimer >= mSettings.timeStep)
    //{
        mPhysicsService.Update(deltaTime);
        //kTimer -= mSettings.timeStep;
    //}
}

//----------------------------------------------------------------------------------------------------

void GameWorld::OnRender()
{
    mTerrainService.Update();
    mRenderService.Update();
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameWorld::CreateGameObject(const char* templateFile, Math::Vector3 pos, Math::Quaternion rot)
{
    // Use the templateFile if it's not null, otherwise use the default object template.
    // Note: For now we're assuming all GameObjects have a transform component
    const char* templatePath = (templateFile) ? templateFile : "../Data/GameObjects/default.json";
    GameObjectHandle handle = mFactory.Create(templatePath);
    if (!handle.IsValid())
    {
        return handle;
    }

    GameObject* gameObject = handle.Get();
    TransformComponent* transformComponent = nullptr;
    if (gameObject->GetComponent(transformComponent))
    {
        transformComponent->Translate(pos);
        // TODO: transformComponent->Rotate(rot);
    }

    // Add the object to the world
    mUpdateList.push_back(handle);
    return handle;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::NewLevel(const char* levelName)
{
    // Clear current level data
    ClearCurrentLevel();

    // Reset the current level data
    mCurrentLevel.path = levelName;
    mCurrentLevel.settings = mSettings;
    mCurrentLevel.buffer = nullptr;
    mCurrentLevel.bufferSize = 0;
    return true;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::LoadLevel(const char* levelName)
{
    // Clear current level data
    ClearCurrentLevel();

    // 1 level = 1 file for now
    // level loader will give us a vector of Level objects containing the buffers for each gameobject and the game settings
    // we then loop through each and call mFactory.Create(buffer) to make the object
    // then we delete the level buffers. TODO: have levelloader hold a managed pool that levels are loaded into.
    // If the level fails to load, crash?

    Level level;
    if (mLevelLoader.Load(levelName, level))
    {
        mSettings = level.settings;
        SerialReader reader(level.buffer, level.bufferSize);
        for (u32 i=0; i < level.numGameObjects; ++i)
        {
            GameObjectHandle handle = mFactory.Create(reader);
            if (!handle.IsValid())
            {
                return false;
            }
            mUpdateList.push_back(handle);
        }
        // Set the current level now that everything has loaded successfully
        mCurrentLevel = level;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::ReLoadCurrentLevel()
{
    // Clear current level data
    ClearCurrentLevel();

    if (mLevelLoader.LoadLocal(mCurrentLevel))
    {
        SerialReader reader(mCurrentLevel.buffer, mCurrentLevel.bufferSize);
        for (u32 i=0; i < mCurrentLevel.numGameObjects; ++i)
        {
            GameObjectHandle handle = mFactory.Create(reader);
            if (!handle.IsValid())
            {
                return false;
            }
            mUpdateList.push_back(handle);
        }
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::SaveLevel(const char* levelName)
{
    return mLevelLoader.SaveToFile(levelName, mUpdateList, mSettings);
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::SaveLevelToMemory()
{
    return mLevelLoader.SaveLocal(mUpdateList, mSettings);
}

//----------------------------------------------------------------------------------------------------

s32 GameWorld::GetScreenWidth() const
{
    return mpApplication->GetScreenWidth();
}

//----------------------------------------------------------------------------------------------------

s32 GameWorld::GetScreenHeight() const
{
    return mpApplication->GetScreenHeight();
}

//----------------------------------------------------------------------------------------------------

void GameWorld::ClearCurrentLevel()
{
    for (Service* service : mServiceList)
    {
        service->UnSubscribeAll();
    }
    mUpdateList.clear();
    mDestroyedList.clear(); // just in case
    mGameObjectPool.Flush();
}

//----------------------------------------------------------------------------------------------------

bool GameWorld::OnGameObjectDestroyed(GameObjectHandle handle)
{
    if (!handle.IsValid())
    {
        return false;
    }

    // Flag for destruction
    //GameObject* gameObject = handle.Get();
    //gameObject->mToBeDestroyed = true;
    //mDestroyedList.push_back(handle);
    //return true;

    auto it = std::find(mUpdateList.begin(), mUpdateList.end(), handle);
    if (&it != nullptr)
    {
        mUpdateList.erase(it);
        return true;
    }
    return false;
}