#include "Precompiled.h"
#include "GameObjectJSON.h"

#include <fstream>

GameObjectJSON::GameObjectJSON()
    : mIsLoaded(false)
{
}

//----------------------------------------------------------------------------------------------------

GameObjectJSON::~GameObjectJSON()
{
}

//----------------------------------------------------------------------------------------------------

bool GameObjectJSON::Load(const std::string& path)
{
    mIsLoaded = false;

    std::ifstream indata(path);
    if (indata.bad())
    {
        return false;
    }

    mIsLoaded = mReader.parse(indata, mRoot);
    return mIsLoaded;
}

void GameObjectJSON::GetComponentProperty(const std::string& componentName, const std::string& propertyName, std::string& data)
{
    if (mIsLoaded)
    {

    }
}

void GameObjectJSON::GetComponentProperty(const std::string& componentName, const std::string& propertyName, Math::Vector3& data)
{
}