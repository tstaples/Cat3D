#ifndef INCLUDED_ENGINE_CAMERA_H
#define INCLUDED_ENGINE_CAMERA_H

//====================================================================================================
// Filename:	Camera.h
// Created by:	Peter Chan
// Description:	Class for a 3D camera.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Camera
{
public:
	Camera();

	void Setup(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetLookAt(const Math::Vector3& target);

	void Walk(f32 distance);
	void Strafe(f32 distance);
	void Rise(f32 distance);

	void Yaw(f32 degree);
	void Pitch(f32 degree);

	f32 GetFOV() const							{ return mFOV; }
	f32 GetAspectRatio() const					{ return mAspectRatio; }
	f32 GetNearPlane() const					{ return mNearPlane; }
	f32 GetFarPlane() const						{ return mFarPlane; }

    const Math::Vector3& GetPosition() const    { return mPosition; }
    const Math::Vector3& GetLookAt() const      { return mLook; }
	
    // TODO: Store these as members
	Math::Matrix GetViewMatrix() const;
	Math::Matrix GetProjectionMatrix() const;

    // Converts 3D world coords into 2D screen coords
    Math::Vector2 Unproject(const Math::Vector3& pos);

    Math::Vector3 ScreenToWorld(s32 mx, s32 my, u32 screenW, u32 screenH) const;
    Math::Ray GetMouseRay(s32 mx, s32 my, u32 screenW, u32 screenH) const;

    Math::Plane GetAxisViewPlane() const;

private:
	void Renormalize();

private:
    Math::Matrix mView;
    Math::Matrix mProjection;

	Math::Vector3 mPosition;
	Math::Vector3 mLook;
	Math::Vector3 mRight;

	f32 mFOV;
	f32 mAspectRatio;
	f32 mNearPlane;
	f32 mFarPlane;
};

#endif // #ifndef INCLUDED_ENGINE_CAMERA_H