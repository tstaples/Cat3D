//====================================================================================================
// Filename:	Camera.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Camera.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mLook(0.0f, 0.0f, 1.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mFOV(Math::kPiByTwo)
	, mAspectRatio(4.0f / 3.0f)
	, mNearPlane(0.01f)
	, mFarPlane(1000.0f)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void Camera::Setup(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane)
{
	mFOV = fov;
	mAspectRatio = aspectRatio;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetDirection(const Math::Vector3& direction)
{
	mLook = direction;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::SetLookAt(const Math::Vector3& target)
{
	mLook = target - mPosition;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Walk(f32 distance)
{
	mPosition = mPosition + (mLook * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Strafe(f32 distance)
{
	mPosition = mPosition + (mRight * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Rise(f32 distance)
{
    Math::Vector3 up = Math::Cross(mLook, mRight);
	mPosition = mPosition + (up * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Yaw(f32 degree)
{
	const Math::Matrix matRotate = Math::Matrix::RotationY(degree * Math::kDegToRad);
	mLook = Math::TransformNormal(mLook, matRotate);
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Pitch(f32 degree)
{
	const Math::Matrix matRotate = Math::Matrix::RotationAxis(mRight, degree * Math::kDegToRad);
	const Math::Vector3 newLook = Math::TransformNormal(mLook, matRotate);
	const f32 dot = Math::Dot(newLook, Math::Vector3::YAxis());

	// Avoid looking straight up or down
	if (Math::Abs(dot) < 0.995f)
	{
		mLook = newLook;
		Renormalize();
	}
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mLook;
	const Math::Vector3 r = mRight;
	const Math::Vector3 u = Math::Normalize(Math::Cross(mLook, mRight));
	const f32 dx = -Math::Dot(r, mPosition);
	const f32 dy = -Math::Dot(u, mPosition);
	const f32 dz = -Math::Dot(l, mPosition);

	return Math::Matrix
	(
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx,  dy,  dz,  1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Camera::GetProjectionMatrix() const
{
	const f32 h = 1 / tan(mFOV * 0.5f);
	const f32 w = h / mAspectRatio;
	const f32 f = mFarPlane;
	const f32 n = mNearPlane;
	const f32 d = f / (f - n);

	return Math::Matrix
	(
		w,    0.0f, 0.0f,   0.0f,
		0.0f, h,    0.0f,   0.0f,
		0.0f, 0.0f, d,      1.0f,
		0.0f, 0.0f, -n * d, 0.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Camera::GetOrthographicProjectionMatrix(u32 scrw, u32 scrh) const
{
	const f32 f = mFarPlane;
	const f32 n = mNearPlane;

    const f32 h = 1 / tan(mFOV * 0.5f);
	const f32 w = h / mAspectRatio;

    XMMATRIX  d3dMat =   XMMatrixOrthographicLH((float)scrw, (float)scrh, n, f);

    return /*Math::Transpose*/(Math::Matrix
	(
		d3dMat.r[0].m128_f32[0],    d3dMat.r[0].m128_f32[1], d3dMat.r[0].m128_f32[2], d3dMat.r[0].m128_f32[3],
		d3dMat.r[1].m128_f32[0],    d3dMat.r[1].m128_f32[1], d3dMat.r[1].m128_f32[2], d3dMat.r[1].m128_f32[3],
		d3dMat.r[2].m128_f32[0],    d3dMat.r[2].m128_f32[1], d3dMat.r[2].m128_f32[2], d3dMat.r[2].m128_f32[3],
	    d3dMat.r[3].m128_f32[0],    d3dMat.r[3].m128_f32[1], d3dMat.r[3].m128_f32[2], d3dMat.r[3].m128_f32[3]
	));

    /*return Math::Matrix
	(
		2.0f/w,  0.0f,       0.0f,        0.0f,
		0.0f,       2.0f/h,  0.0f,        0.0f,
		0.0f,       0.0f,       1/(f-n),     0.0f,
	    0.0f,       0.0f,       n/(n-f),     1.0f
	);*/
}

//----------------------------------------------------------------------------------------------------

void Camera::Renormalize()
{
	mLook = Math::Normalize(mLook);
	mRight = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), mLook));
}

//----------------------------------------------------------------------------------------------------

Math::Vector2 Camera::WorldToScreen(const Math::Vector3& pos, u32 screenW, u32 screenH)
{
    Math::Matrix view = GetViewMatrix();
    Math::Matrix proj = GetProjectionMatrix();
    Math::Vector3 worldView = Math::TransformCoord(pos, view);
    Math::Vector3 hcspos = Math::TransformCoord(worldView, proj);

    Math::Vector2 screenPos;
    const f32 halfW = screenW * 0.5f;
    const f32 halfH = screenH * 0.5f;

    screenPos.x = halfW * hcspos.x + halfW;
    screenPos.y = -halfH * hcspos.y + halfH;
    return screenPos;
}

//----------------------------------------------------------------------------------------------------
Math::Vector2 Camera::WorldToScreenOrthographic(const Math::Vector3& pos, u32 screenW, u32 screenH)
{
    Math::Matrix view = GetViewMatrix();
    Math::Matrix proj = GetOrthographicProjectionMatrix(screenW, screenH);
    Math::Vector3 worldView = Math::TransformCoord(pos, view);
    Math::Vector3 hcspos = Math::TransformCoord(worldView, proj);

    Math::Vector2 screenPos;
    const f32 halfW = screenW * 0.5f;
    const f32 halfH = screenH * 0.5f;

    screenPos.x = halfW * hcspos.x + halfW;
    screenPos.y = -halfH * hcspos.y + halfH;
    return screenPos;
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 Camera::ScreenToWorld(s32 mx, s32 my, u32 screenW, u32 screenH) const
{
    // Transform 2d mouse coords to NDC
    Math::Vector3 mouseNDC;
    mouseNDC.x = (2.0f * (f32)mx) / (f32)screenW - 1.0f;
    mouseNDC.y = 1.0f - (2.0f * (f32)my) / (f32)screenH;
    mouseNDC.z = 0.0f;

    // Transform the mouse NDC coords into world space
    Math::Matrix projection     = GetProjectionMatrix();
    Math::Matrix invProjection  = Math::Inverse(projection);
    Math::Matrix cameraView     = Math::Inverse(GetViewMatrix());
    Math::Matrix transform      = invProjection * cameraView;
    Math::Vector3 mouseWorld    = Math::TransformCoord(mouseNDC, transform);
    return mouseWorld;
}

//----------------------------------------------------------------------------------------------------

Math::Ray Camera::GetMouseRay(s32 mx, s32 my, u32 screenW, u32 screenH) const
{
    Math::Vector3 mouseWorld = ScreenToWorld(mx, my, screenW, screenH);
    Math::Vector3 dir = Math::Normalize(mouseWorld - mPosition);
    return Math::Ray(mPosition, dir);
}

//----------------------------------------------------------------------------------------------------

Math::Plane Camera::GetAxisViewPlane() const
{
    Math::Plane px(Math::Vector3::XAxis(), 0.0f);
    Math::Plane py(Math::Vector3::YAxis(), 0.0f);
    Math::Plane pz(Math::Vector3::ZAxis(), 0.0f);

    f32 dotX = fabsf(Math::Dot(mLook, px.n));
    f32 dotY = fabsf(Math::Dot(mLook, py.n));
    f32 dotZ = fabsf(Math::Dot(mLook, pz.n));

    if (dotX > dotY && dotX > dotZ)
        return px;
    else if (dotY > dotX && dotY > dotZ)
        return py;
    return pz;
}