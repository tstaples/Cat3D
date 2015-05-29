#include "Precompiled.h"
#include "Gizmo.h"

#include "EditorObject.h"
#include "InputData.h"

namespace
{
    enum Axis : u8
    {
        X = 1,
        Y = 2,
        Z = 4
    };

    void BuildArms(f32 ext, f32 width, Math::AABB& x, Math::AABB& y, Math::AABB& z)
    {
        const f32 halfExt = ext * 0.5f;
        const f32 halfWidth = width * 0.5f;

        x.center = Math::Vector3(halfExt, 0.0f, 0.0f);
        y.center = Math::Vector3(0.0f, halfExt, 0.0f);
        z.center = Math::Vector3(0.0f, 0.0f, halfExt);
        x.extend = Math::Vector3(halfExt, halfWidth, halfWidth);
        y.extend = Math::Vector3(halfWidth, halfExt, halfWidth);
        z.extend = Math::Vector3(halfWidth, halfWidth, halfExt);
    }

    Math::Vector3 GetAxisTranslation(u8 axis, const Math::Vector3& v)
    {
        Math::Vector3 dist;
        if (axis & Axis::X) dist.x = v.x;
        if (axis & Axis::Y) dist.y = v.y;
        if (axis & Axis::Z) dist.z = v.z;
        return dist;
    }

    Math::Vector3 GetTranslationAxis(u8 axis)
    {
        Math::Vector3 taxis;
        if (axis & Axis::X) taxis = Math::Vector3::XAxis();
        if (axis & Axis::Y) taxis = Math::Vector3::YAxis();
        if (axis & Axis::Z) taxis = Math::Vector3::ZAxis();
        return taxis;
    }

    Math::Vector3 GetMidPoint(const Gizmo::Objects& objs)
    {
        Math::Vector3 center;
        if (objs.size() == 1)
        {
            return objs.front()->GetPosition();
        }
        for (EditorObject* obj : objs)
        {
            const Math::Vector3& pos = obj->GetPosition();
            center += (center + pos) * 0.5f;
        }
        return center;
    }

    Math::Plane GetTranslationPlane(u8 selectedAxis, const Math::Vector3& pos, const Math::Ray& mouseRay)
    {
        Math::Plane px(Math::Vector3::XAxis(), pos.x);
        Math::Plane py(Math::Vector3::YAxis(), pos.y);
        Math::Plane pz(Math::Vector3::ZAxis(), pos.z);
        
        // eliminate one of the planes based on the selected axis
        Math::Plane a, b;
        if (selectedAxis == Axis::X)
        {
            a = py;
            b = pz;
        }
        else if (selectedAxis == Axis::Y)
        {
            a = px;
            b = pz;
        }
        else
        {
            a = px;
            b = py;
        }
        // dot ray dir with normal of 2 remaining planes
        f32 dotA = Math::Dot(mouseRay.dir, a.n);
        f32 dotB = Math::Dot(mouseRay.dir, b.n);

        // use plane whose abs result was greater.
        return (fabsf(dotA) > fabsf(dotB)) ? a : b;
    }
}

//----------------------------------------------------------------------------------------------------

Gizmo::Gizmo(Camera& camera, f32 extend, f32 armWidth)
    : mCamera(camera)
    , mExtend(extend)
    , mArmWidth(armWidth)
    , mSelectedArm(0)
{
}

//----------------------------------------------------------------------------------------------------

Gizmo::~Gizmo()
{
}

//----------------------------------------------------------------------------------------------------

// https://www.opengl.org/discussion_boards/showthread.php/144271-Rendering-gizmo-(3d-handler)
f32 Gizmo::CalcScaleFactor(const Math::Vector3& pos) const
{
    const Math::Vector3& cameraPos = mCamera.GetPosition();
    f32 dist = Math::Distance(pos, cameraPos);
    f32 fov = mCamera.GetFOV();
    const f32 magicNumber = 0.25f;
    f32 s = (atanf(fov) * dist) * magicNumber;
    return s;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

TranslateGizmo::TranslateGizmo(Camera& camera, f32 extend, f32 armWidth)
    : Gizmo(camera, extend, armWidth)
{
}

//----------------------------------------------------------------------------------------------------

TranslateGizmo::~TranslateGizmo()
{
}

//----------------------------------------------------------------------------------------------------

bool TranslateGizmo::CheckSelection(const Objects& selectedObjs, const Math::Ray& mouseRay)
{
    // Note: using global space. TODO: local space as well

    // The transform is just the translation to the midpoint of all the objects
    Math::Vector3 center = GetMidPoint(selectedObjs);
    Math::Matrix transform = Math::Matrix::Translation(center);

    Math::Matrix toObject = Math::Inverse(transform);
    Math::Vector3 mouseLocalPos = Math::TransformCoord(mouseRay.org, toObject);
    Math::Vector3 mouseLocalDir = Math::TransformNormal(mouseRay.dir, toObject);
    Math::Ray localRay(mouseLocalPos, mouseLocalDir);

    f32 scale = CalcScaleFactor(center);
    f32 ext = mExtend * scale;

    // First pass test to check if mouse is even within the bounds of the gizmo
    Math::AABB aabb(Math::Vector3::Zero(), Math::Vector3(ext, ext, ext));
    if (!Math::Intersect(localRay, aabb))
    {
        return false;
    }

    // Get the colliders for the arms in local space
    f32 width = mArmWidth;// * scale;
    Math::AABB armX, armY, armZ;
    BuildArms(ext, mArmWidth, armX, armY, armZ);

    // Mark which axes are selected for later use
    mSelectedArm = 0; // reset
    if (Math::Intersect(localRay, armX))
    {
        mSelectedArm |= Axis::X;
    }
    else if (Math::Intersect(localRay, armY))
    {
        mSelectedArm |= Axis::Y;
    }
    else if (Math::Intersect(localRay, armZ))
    {
        mSelectedArm |= Axis::Z;
    }
    return (mSelectedArm != 0);
}

//----------------------------------------------------------------------------------------------------

void TranslateGizmo::Update(const Objects& selectedObjs, const InputData& input, u32 screenW, u32 screenH)
{
    if (selectedObjs.empty())
        return;

    // Find the center of all the selected object in world space
    Math::Vector3 center = GetMidPoint(selectedObjs);

    if (mSelectedArm > 0)
    {
        // Get the mouse's previous position
        s32 mx = input.mouseX, my = input.mouseY;
        f32 mdx = input.mouseMoveX, mdy = input.mouseMoveY;
        s32 dx = mx - (s32)mdx;
        s32 dy = my - (s32)mdy;

        // No mouse movement
        if (Math::IsZero(mdx) && Math::IsZero(mdy))
            return;

        Math::Ray mouseRay = mCamera.GetMouseRay(mx, my, screenW, screenH);
        Math::Ray prevMouseRay = mCamera.GetMouseRay(dx, dy, screenW, screenH);
        Math::Plane transPlane = GetTranslationPlane(mSelectedArm, center, mouseRay);
        Math::Vector3 intersect, oldIntersect;
        Math::GetIntersectPoint(prevMouseRay, transPlane, oldIntersect);
        Math::GetIntersectPoint(mouseRay, transPlane, intersect);
        center = intersect - oldIntersect;

        Math::Vector3 translation = GetAxisTranslation(mSelectedArm, center);
        for (EditorObject* obj : selectedObjs)
        {
            // Translate each object
            obj->Translate(translation);
        }
    }
}

//----------------------------------------------------------------------------------------------------

void TranslateGizmo::Draw(const Objects& selectedObjs)
{
    // Only global supported currently, so only need translation.
    Math::Vector3 center = GetMidPoint(selectedObjs);
    Math::Matrix transform = Math::Matrix::Translation(center);

    f32 scale = CalcScaleFactor(center);
    f32 ext = mExtend * scale;

    Math::Vector3 xAxis = Math::TransformCoord(Math::Vector3::XAxis() * ext, transform);
    Math::Vector3 yAxis = Math::TransformCoord(Math::Vector3::YAxis() * ext, transform);
    Math::Vector3 zAxis = Math::TransformCoord(Math::Vector3::ZAxis() * ext, transform);

    // TODO: De-select arms on mouseup
    Color xcol = (mSelectedArm & Axis::X) ? Color::Yellow() : Color::Red();
    Color ycol = (mSelectedArm & Axis::Y) ? Color::Yellow() : Color::Green();
    Color zcol = (mSelectedArm & Axis::Z) ? Color::Yellow() : Color::Blue();
    SimpleDraw::AddLine(center, xAxis, xcol);
    SimpleDraw::AddLine(center, yAxis, ycol);
    SimpleDraw::AddLine(center, zAxis, zcol);


    // debug
    f32 width = mArmWidth;// * scale;
    const f32 halfExt = ext * 0.5f;
    Math::AABB armX, armY, armZ;
    BuildArms(ext, width, armX, armY, armZ);
    armX.center = center; armX.center.x += halfExt;
    armY.center = center; armY.center.y += halfExt;
    armZ.center = center; armZ.center.z += halfExt;
    SimpleDraw::AddAABB(armX, Color::Cyan());
    SimpleDraw::AddAABB(armY, Color::Cyan());
    SimpleDraw::AddAABB(armZ, Color::Cyan());
}