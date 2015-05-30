#include "EditorObject.h"

EditorObject::EditorObject(GameObjectHandle gameObjectHandle)
    : mGameObjectHandle(gameObjectHandle)
    , mpParent(nullptr)
    , mpChildren(nullptr)
    , mNumChildren(0)
    , mIsSelected(false)
{
    mDefaultCollider = Math::AABB(GetPosition(), Math::Vector3(5.0f, 5.0f, 5.0f));
    f32 extend = 10.0f;
}

//----------------------------------------------------------------------------------------------------

EditorObject::~EditorObject()
{
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 EditorObject::GetPosition() const
{
    const TransformComponent* transformComponent = nullptr;
    if (!mGameObjectHandle.Get()->FindComponent(transformComponent))
    {
        return Math::Vector3();
    }
    return transformComponent->GetPosition();
}

//----------------------------------------------------------------------------------------------------

Math::AABB EditorObject::GetCollider() const
{
    const GameObject* gameObject = mGameObjectHandle.Get();
    const ColliderComponent* collider = nullptr;
    if (gameObject->FindComponent(collider))
    {
        return collider->GetBoundary();
    }
    return Math::AABB(GetPosition(), Math::Vector3(5.0f, 5.0f, 5.0f));
}

//----------------------------------------------------------------------------------------------------

void EditorObject::Translate(const Math::Vector3& t)
{
    TransformComponent* transformComponent = nullptr;
    if (!mGameObjectHandle.Get()->FindComponent(transformComponent))
    {
        return;
    }

    // Translate the GameObject
    transformComponent->Translate(t);
    // Move the collider to the object's new position
    mDefaultCollider.center = transformComponent->GetPosition();
}