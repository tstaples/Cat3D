#ifndef INCLUDED_ENGINE_GAMEWORLD_H
#define INCLUDED_ENGINE_GAMEWORLD_H

//====================================================================================================
// Filename:	GameWorld.h
// Created by:	Tyler Staples
// Description:	Class representing the game world. The main benefit of this class is to be able to
// abstract the game related functionality and systems away from the application side.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Components.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameSettings.h"
#include "LevelLoader.h"
#include "Services.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;
class Application;

//====================================================================================================
// Class Declarations
//====================================================================================================

typedef std::vector<GameObjectHandle> GameObjectHandles;

class GameWorld
{
public:
    GameWorld(Application* app, u16 maxObjects);
    ~GameWorld();

    // Note: camera param temp until camera component created
    bool OnInitialize(const GameSettings& settings, GraphicsSystem& gs, Camera& camera);
    bool OnShutdown();
    bool OnInput();
    void OnUpdate(f32 deltaTime);
    void OnRender();

    /* Creates and adds a new GameObject to the world.
     * @param templateFile: template to create the object from. Pass in nullptr to create and empty object.
     * @param pos: position in world space the game object will be placed.
     * @param rot: rotation of the object in world space.
     * Returns the handle to the newly created GameObject.
     */
    GameObjectHandle CreateGameObject(const char* templateFile, Math::Vector3 pos, Math::Quaternion rot);

    bool NewLevel(const char* levelName);
    bool LoadLevel(const char* levelName);
    bool SaveLevel(const char* levelName);

    s32 GetScreenWidth() const;
    s32 GetScreenHeight() const;

    const GameSettings& GetGameSettings() const { return mSettings; }

private:
    // Delegates
    bool OnGameObjectDestroyed(GameObjectHandle handle);

    bool SaveLevelToMemory();
    bool ReLoadCurrentLevel();
    void ClearCurrentLevel();

private:
    NONCOPYABLE(GameWorld);

    Application* mpApplication;

    GameSettings mSettings;

    // Services
    Services mServiceList; // used for iterating and calling generic methods
    RenderService mRenderService;
    PhysicsService mPhysicsService;

    GameObjectFactory mFactory;
    GameObjectPool mGameObjectPool;

    // TODO: Hold gameObject pointers for faster access
    // then get the handle through the gameObject
    GameObjectHandles mUpdateList;
    GameObjectHandles mDestroyedList;

    LevelLoader mLevelLoader;
    Level mCurrentLevel;

private:
    // Only the editor will need to directly access the GameWorld's members
    friend class EditorApp;
    friend class EditorCommands;
    friend class TestApp; // temp
};

#endif // #ifndef INCLUDED_ENGINE_GAMEWORLD_H