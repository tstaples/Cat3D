#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

META_CLASS_BEGIN(GameObject)
META_CLASS_END

GameObject::GameObject()
    : mName("Unknown")
    //, mTransform(this)
{
}

//----------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddComponent(Component* component)
{
    mComponents.push_back(component);
}