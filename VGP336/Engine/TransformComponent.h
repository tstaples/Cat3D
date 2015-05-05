#ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_ENGINE_TRANSFORMCOMPONENT_H

//====================================================================================================
// Filename:	TransformComponent.h
// Created by:	Tyler Staples
// Description: Class representing Transform Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class TransformComponent : public Component
{
public:
    TransformComponent();
    TransformComponent(const Math::Matrix& transform);
    ~TransformComponent();

    void Translate(const Math::Vector3& t);

    Math::Vector3 GetPosition() const;

    Math::Matrix& GetTransform()                { return mTransform; }
    const Math::Matrix& GetTransform() const    { return mTransform; }

    void SetTransform(const Math::Matrix& transform) { mTransform = transform; }

    virtual void Load(Json::Value& properties);

private:
    Math::Matrix mTransform;
};

#endif // #ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H