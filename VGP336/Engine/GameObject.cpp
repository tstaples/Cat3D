#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject()
    : mName("Unknown")
{
}

//----------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
}

//----------------------------------------------------------------------------------------------------

//void GameObject::AddComponent(ID id)
//{
//    // Insert won't throw an error, but it's helpful to know if we're adding something more than once
//    ASSERT(mComponents.find(id.GetType()) == mComponents.end(), 
//        "[GameObject] Component of type: %u already exists", id.GetType());
//
//    mComponents.insert(std::make_pair(id.GetType(), id));
//}
//
////----------------------------------------------------------------------------------------------------
//
//void GameObject::AddService(Meta::Type type)
//{
//    ASSERT(std::find(mServices.begin(), mServices.end(), type) == mServices.end(), 
//        "[GameObject] Service of type: %u already exists", type);
//
//    mServices.push_back(type);
//}
//
////----------------------------------------------------------------------------------------------------
//
//ID GameObject::GetComponentID(Meta::Type type)
//{
//    auto component = mComponents.find(type);
//    ASSERT(component != mComponents.end(), "[GameObject] Component of type: %u not found", type);
//    return component->second;
//}
//
////----------------------------------------------------------------------------------------------------
//
//ID GameObject::FindComponentID(Meta::Type type)
//{
//    ID id;
//    auto component = mComponents.find(type);
//    if (component != mComponents.end())
//    {
//        id = component->second;
//    }
//    return id;
//}