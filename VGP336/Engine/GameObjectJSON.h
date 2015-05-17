#ifndef INCLUDED_ENGINE_GAMEOBJECTJSON_H
#define INCLUDED_ENGINE_GAMEOBJECTJSON_H

//====================================================================================================
// Filename:	GameObjectJSON.h
// Created by:	Tyler Staples
// Description: Class representing the JSON data for a GameObject. Wraps jsoncpp.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

#include <json/json.h>

//====================================================================================================
// Class Declarations
//====================================================================================================


class GameObjectJSON
{
public:
    GameObjectJSON();
    ~GameObjectJSON();

    bool Load(const std::string& path);

    void GetComponentProperty(const std::string& componentName, const std::string& propertyName, std::string& data);
    void GetComponentProperty(const std::string& componentName, const std::string& propertyName, Math::Vector3& data);

private:
    bool mIsLoaded;

    Json::Value mRoot;
    Json::Reader mReader;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECTJSON_H