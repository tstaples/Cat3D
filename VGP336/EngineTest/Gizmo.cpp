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

TranslateGizmo::TranslateGizmo(Camera& camera, f32 extend, f32 armWidth)
    : Gizmo(camera, extend, armWidth)
{
}

//----------------------------------------------------------------------------------------------------

TranslateGizmo::~TranslateGizmo()
{
}

//----------------------------------------------------------------------------------------------------

bool TranslateGizmo::IsSelected(const Objects& selectedObjs, const Math::Ray& mouseRay)
{
    // Note: using global space. TODO: local space as well

    // The transform is just the translation to the midpoint of all the objects
    Math::Vector3 center = GetMidPoint(selectedObjs);
    Math::Matrix transform = Math::Matrix::Translation(center);

    Math::Matrix toObject = Math::Inverse(transform);
    Math::Vector3 mouseLocalPos = Math::TransformCoord(mouseRay.org, toObject);
    Math::Vector3 mouseLocalDir = Math::TransformNormal(mouseRay.dir, toObject);
    Math::Ray localRay(mouseLocalPos, mouseLocalDir);

    // First pass test to check if mouse is even within the bounds of the gizmo
    Math::AABB aabb(Math::Vector3::Zero(), Math::Vector3(mExtend, mExtend, mExtend));
    if (!Math::Intersect(localRay, aabb))
    {
        return false;
    }

    // Get the colliders for the arms in local space
    Math::AABB armX, armY, armZ;
    BuildArms(mExtend, mArmWidth, armX, armY, armZ);

    mSelectedArm = 0; // reset
    if (Math::Intersect(localRay, armX))
    {
        mSelectedArm |= 1;
    }
    if (Math::Intersect(localRay, armY))
    {
        mSelectedArm |= 2;
    }
    if (Math::Intersect(localRay, armZ))
    {
        mSelectedArm |= 5;
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
    const Math::Vector3& cameraPos = mCamera.GetPosition();
    f32 dist = Math::Distance(cameraPos, center);

    if (mSelectedArm > 0)
    {
        f32 tempMagicNumber = 5.0f;

        // Get the mouse's previous position
        s32 mx = input.mouseX, my = input.mouseY;
        f32 mdx = input.mouseMoveX, mdy = input.mouseMoveY;
        s32 dx = mx - (s32)mdx * tempMagicNumber;
        s32 dy = my - (s32)mdy * tempMagicNumber;

        // http://www.blitzmax.com/Community/posts.php?topic=75102
        // http://gamedev.stackexchange.com/questions/79972/how-to-implement-translation-scale-rotation-gizmos-for-manipulating-3d-object
        //if (dx != 0 || dy != 0)
        if (mdx != 0.0f || mdy != 0.0f)
        {
            // Get the difference between the current and previous mouse world positions and translate by that
            // in the corresponding axes.
            Math::Vector3 mouseWorld = mCamera.GetMouseWorld(mx, my, screenW, screenH);
            Math::Vector3 prevMouseWorld  = mCamera.GetMouseWorld(dx, dy, screenW, screenH);
            Math::Vector3 diff = mouseWorld - prevMouseWorld;
            Math::Vector3 translation = GetAxisTranslation(mSelectedArm, diff);

            for (EditorObject* obj : selectedObjs)
            {
                // Translate each object
                obj->Translate(translation);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void TranslateGizmo::Draw(const Objects& selectedObjs)
{
    // Only global supported currently, so only need translation.
    Math::Vector3 center = GetMidPoint(selectedObjs);
    Math::Matrix transform = Math::Matrix::Translation(center);

    Math::Vector3 xAxis = Math::TransformCoord(Math::Vector3::XAxis() * mExtend, transform);
    Math::Vector3 yAxis = Math::TransformCoord(Math::Vector3::YAxis() * mExtend, transform);
    Math::Vector3 zAxis = Math::TransformCoord(Math::Vector3::ZAxis() * mExtend, transform);

    SimpleDraw::AddLine(center, xAxis, Color::Green());
    SimpleDraw::AddLine(center, yAxis, Color::Yellow());
    SimpleDraw::AddLine(center, zAxis, Color::Blue());

    // debug
    const f32 halfExt = mExtend * 0.5f;
    Math::AABB armX, armY, armZ;
    BuildArms(mExtend, mArmWidth, armX, armY, armZ);
    armX.center = center; armX.center.x += halfExt;
    armY.center = center; armY.center.y += halfExt;
    armZ.center = center; armZ.center.z += halfExt;
    SimpleDraw::AddAABB(armX, Color::Blue());
    SimpleDraw::AddAABB(armY, Color::Blue());
    SimpleDraw::AddAABB(armZ, Color::Blue());
}