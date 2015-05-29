#ifndef INCLUDED_ENGINE_CAMERACOMPONENT_H
#define INCLUDED_ENGINE_CAMERACOMPONENT_H

//====================================================================================================
// Filename:	CameraComponent.h
// Created by:	Tyler Staples
// Description: Class representing a Camera Component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Component.h"
#include "EngineMath.h"
#include "Camera.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class CameraComponent : public Component
{
public:
    META_DECLARE_CLASS

    CameraComponent();
    ~CameraComponent();

    virtual void Update(f32 deltaTime) override;

    const Camera& GetCamera() const { return mCamera; }

private:
    f32 mFov;
    f32 mNear;
    f32 mFar;
    Camera mCamera;
};

//====================================================================================================
// Typedefs
//====================================================================================================

//typedef MemHandle<TransformComponent> TransformHandle;

#endif // #ifndef INCLUDED_ENGINE_CAMERACOMPONENT_H