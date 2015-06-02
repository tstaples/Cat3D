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

#include "MemoryPool.h"
#include "Meta.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Component;
class GameWorld;

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
    GameObject(const char* name);
    ~GameObject();

    void Update(f32 deltaTime);
    void AddComponent(Component* component);

    void AddService(u16 id);
    void RemoveService(u16 id);
    bool HasService(u16 id) const;

    // Returns bool rather than pointer to force caching local pointer
    template<typename T>
    bool GetComponent(T*& component);
    template<typename T>
    bool GetComponent(const T*& component) const;

    template<typename T>
    bool FindComponent(T*& component);
    template<typename T>
    bool FindComponent(const T*& component) const;

    bool HasComponent(const char* componentName) const;

    void SetName(const char* name)              { mName = name; }
    const char* GetName() const                 { return mName.c_str(); }
    const Components& GetComponents() const     { return mComponents; }

    bool IsValid() const                        { return !mToBeDestroyed; }

    GameWorld* GetWorld() const;

    bool Serialize(SerialWriter& writer);
    bool Deserialize(SerialReader& reader);

private:
    //NONCOPYABLE(GameObject)
    friend class GameObjectFactory;
    friend class GameWorld;

    std::string mName; // See TODO in TString
    Components mComponents;

    static const u16 kNumServices = 2; // TODO: find better way to do this
    bool mServiceSubscriptions[kNumServices];

    bool mToBeDestroyed;  // Is the object in the update list
    bool mEnabled; // TODO

    static class GameWorld* spWorld;
};

//====================================================================================================
// Typedefs
//====================================================================================================

typedef MemHandle<GameObject> GameObjectHandle;
typedef MemoryPool<GameObject> GameObjectPool;

//====================================================================================================
// Inline Definitions
//====================================================================================================

// Create a specialization to validate so we can ensure the actual gameObject is valid too.
template <>
inline bool MemHandle<GameObject>::IsValid() const
{
    return sMemoryPool &&
           sMemoryPool->IsValid(*this) &&
           sMemoryPool->Get(*this)->IsValid();
}

//----------------------------------------------------------------------------------------------------

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