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
#include "Service.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;
class Level;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameWorld
{
    typedef std::vector<GameObjectHandle> GameObjectHandles;

public:
    GameWorld(u16 maxObjects);
    ~GameWorld();

    // Note: camera param temp until camera component created
    bool OnInitialize(const GameSettings& settings, GraphicsSystem& gs, Camera& camera);
    bool OnShutdown();
    bool OnInput();
    void OnUpdate(f32 deltaTime);
    void OnRender();

    // Creates and adds a new GameObject to the world.
    // @param templateFile: template to create the object from. Pass in nullptr to create and empty object.
    // @param pos: position in world space the game object will be placed.
    // @param rot: rotation of the object in world space.
    void CreateGameObject(const char* templateFile, Math::Vector3 pos, Math::Quaternion rot);

    bool LoadLevel(const char* levelName);
    //bool SaveCurrentLevel(const char* path)

private:
    NONCOPYABLE(GameWorld);

    GameSettings mSettings;

    // Services
    RenderService mRenderService;

    GameObjectFactory mFactory;
    GameObjectPool mGameObjectPool;

    GameObjectHandles mGameObjectHandles;

    LevelLoader mLevelLoader;

private:
    // Only the editor will need to directly access the GameWorld's members
    friend class EditorApp;
    friend class TestApp; // temp
};

#endif // #ifndef INCLUDED_ENGINE_GAMEWORLD_H