#include "EditorObject.h"

EditorObject::EditorObject(GameObject* gameObject)
    : mpGameObject(gameObject)
    , mpParent(nullptr)
    , mpChildren(nullptr)
    , mNumChildren(0)
    , mIsSelected(false)
{
    mDefaultCollider = Math::AABB(GetPosition(), Math::Vector3(5.0f, 5.0f, 5.0f));
    f32 extend = 10.0f;
    mGizmo = Gizmo(GetPosition(), extend);
}

EditorObject::~EditorObject()
{
}

void EditorObject::DrawGizmo()
{
    const Math::Matrix& transform = mpGameObject->GetTransform().GetTransform();
    mGizmo.Draw(transform);
    //Math::Vector3 xAxis = Math::TransformCoord(Math::Vector3::XAxis() * 10.0f, transform);
    //Math::Vector3 yAxis = Math::TransformCoord(Math::Vector3::YAxis() * 10.0f, transform);
    //Math::Vector3 zAxis = Math::TransformCoord(Math::Vector3::ZAxis() * 10.0f, transform);
    //Math::Vector3 center = mpGameObject->GetTransform().GetPosition();

    //SimpleDraw::AddLine(center, xAxis, Color::Green());
    //SimpleDraw::AddLine(center, yAxis, Color::Yellow());
    //SimpleDraw::AddLine(center, zAxis, Color::Blue());
}

Math::Vector3 EditorObject::GetPosition() const
{
    return mpGameObject->GetTransform().GetPosition();
}

const Math::AABB& EditorObject::GetCollider() const
{
    // TODO: Check if gameobject has a collider component and return that instead
    return mDefaultCollider;
}

void EditorObject::Translate(const Math::Vector3& t)
{
    // Translate the GameObject
    mpGameObject->GetTransform().Translate(t);
    // Move the collider to the object's new position
    mDefaultCollider.center = mpGameObject->GetTransform().GetPosition();
    mGizmo.Translate(t);
    //mGizmo.center = mpGameObject->GetTransform().GetPosition();
}

Math::Vector3 EditorObject::GetSelectedAxis(const Math::Ray& mouseRay) const
{
    Math::Matrix transform = mpGameObject->GetTransform().GetTransform();
    //Math::Vector3 org = Math::TransformCoord(mouseRay.org, transform);
    //Math::Vector3 dir = Math::TransformNormal(mouseRay.dir, transform);
    //Math::Ray ray(org, dir);
    return mGizmo.IsArmSelected(transform, mouseRay);
}