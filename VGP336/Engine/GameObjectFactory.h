#ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_ENGINE_GAMEOBJECTFACTORY_H

//====================================================================================================
// Filename:	GameObjectFactory.h
// Created by:	Tyler Staples
// Description: Class for a GameObject Factory.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "GameObject.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObjectFactory
{
public:
    GameObjectFactory(GameObjectPool& gameObjectPool);
    ~GameObjectFactory();

    GameObjectHandle Create(const char* templateFileName);

private:
    GameObjectPool& mGameObjectPool;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H