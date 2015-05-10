#ifndef INCLUDED_ENGINE_GAMEOBJECT_H
#define INCLUDED_ENGINE_GAMEOBJECT_H

//====================================================================================================
// Filename:	GameObject.h
// Created by:	Tyler Staples
// Description: Class representing a GameObject.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MemHandle.h"
#include "Meta.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Component;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObject
{
public:
    typedef std::vector<Component*> Components;

public:
    META_DECLARE_CLASS

    GameObject();
    ~GameObject();

    void AddComponent(Component* component);

    // Returns bool rather than pointer to force caching local pointer
    template<typename T>
    bool GetComponent(T*& component);
    template<typename T>
    bool GetComponent(const T*& component) const;

    template<typename T>
    bool FindComponent(T*& component);
    template<typename T>
    bool FindComponent(const T*& component) const;

    const char* GetName() const                 { return mName.c_str(); }
    const Components& GetComponents() const     { return mComponents; }

private:
    //NONCOPYABLE(GameObject)

    std::string mName; // See TODO in TString
    Components mComponents;
};

//====================================================================================================
// Typedefs
//====================================================================================================

typedef MemHandle<GameObject> GameObjectHandle;
typedef MemoryPool<GameObject> GameObjectPool;

//====================================================================================================
// Inline Definitions
//====================================================================================================

template<typename T>
bool GameObject::GetComponent(T*& component)
{
    bool result = FindComponent(component);
    ASSERT(result, "[GameObject] could not find component");
    return result;
}

//----------------------------------------------------------------------------------------------------

template<typename T>
bool GameObject::GetComponent(const T*& component) const
{
    bool result = FindComponent(component);
    ASSERT(result, "[GameObject] could not find component");
    return result;
}

//----------------------------------------------------------------------------------------------------

template<typename T>
bool GameObject::FindComponent(T*& component)
{
    component = nullptr;
    for (Component* c : mComponents)
    {
        if (c->GetMetaClass() == T::StaticGetMetaClass())
        {
            component = static_cast<T*>(c);
            break;
        }
    }
    return (component != nullptr);
}

//----------------------------------------------------------------------------------------------------

template<typename T>
bool GameObject::FindComponent(const T*& component) const
{
    component = nullptr;
    for (Component* c : mComponents)
    {
        if (c->GetMetaClass() == T::StaticGetMetaClass())
        {
            component = static_cast<const T*>(c);
            break;
        }
    }
    return (component != nullptr);
}

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECT_H