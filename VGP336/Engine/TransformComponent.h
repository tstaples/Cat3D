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
    META_DECLARE_CLASS

    TransformComponent();
    TransformComponent(const Math::Matrix& transform);
    ~TransformComponent();

    void Translate(const Math::Vector3& t);

    Math::Vector3 GetPosition() const;

    Math::Matrix GetTransform() const;

    void SetTransform(const Math::Matrix& transform) { mTransform = transform; }

private:
    //http://www.gamasutra.com/view/feature/131686/rotating_objects_using_quaternions.php
    //Math::Quaternion mRotation; // TODO
    Math::Vector3 mPosition;
    Math::Vector3 mRotation;
    Math::Vector3 mScale;

    Math::Matrix mTransform;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<TransformComponent> TransformHandle;

#endif // #ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H