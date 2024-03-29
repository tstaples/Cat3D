#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "GameWorld.h"
#include "MemoryPool.h"
#include "IO.h"
#include "RenderService.h"

#include <json/json.h>
#include <fstream>
#include <exception>


namespace
{
    Service* LookUpService(const Services& services, const char* name)
    {
        for (Service* service : services)
        {
            if (strcmp(service->GetName(), name) == 0)
            {
                return service;
            }
        }
        return nullptr;
    }

    //bool CheckServiceDependencies(GameObjectHandle& handle, const char* newComponent, const Service* service)
    //{
    //    const GameObject* gameObject = handle.Get();
    //    // Get the service's dependencies from it's metaclass
    //    const char* serviceName = service->GetName();
    //    const MetaClass* metaClass = MetaDB::GetMetaClass(serviceName);
    //    const u32 numDependencies = metaClass->GetNumDependencies();
    //    const MetaDependency* dependencies = metaClass->GetDependencyList();
    //    for (u32 j=0; j < numDependencies; ++j)
    //    {
    //        // Check if the gameObject already has the dependency, or if it's the item being added
    //        const char* depName = dependencies[j].GetName();
    //        if (!gameObject->HasComponent(depName) && strcmp(newComponent, depName) != 0)
    //        {
    //            throw new Exceptions::MissingDependencyException(handle, depName);
    //        }
    //    }
    //    return true;
    //}

    bool LinkDependency(GameObjectHandle& handle, const Services& services, Component* c)
    {
        const GameObject* gameObject = handle.Get();
        const MetaClass* metaClass = c->GetMetaClass();
        const u32 numDependencies = metaClass->GetNumDependencies();
        if (numDependencies == 0)
        {
            // Nothing to do
            return true;
        }
        
        for (u32 j=0; j < numDependencies; ++j)
        {
            // Check if any of the component's dependencies match the services
            const MetaDependency* metaDependency = metaClass->GetDependencyAtIndex(j);
            const char* depName = metaDependency->GetName();
            const char* depTag = metaDependency->GetTag();

            if (strcmp(depTag, "Service") == 0)
            {
                Service* service = LookUpService(services, depName);
                if (service != nullptr)
                {
                    // Subscribe the GameObject to that service.
                    // Note: not using Subscribe's return value since multiple component may need to be subcribed
                    // to the same service, resulting in false positives.
                    service->Subscribe(handle);
                    return true;
                }
            }
            else if (strcmp(depTag, "Component") == 0)
            {
                // Dependency is a component, so check if the GameObject has it
                return gameObject->HasComponent(depName);
            }
        }
        return false;
    }

    bool LinkDependencies(GameObjectHandle& handle, const Services& services)
    {
        GameObject* gameObject = handle.Get();
        for (Component* c : gameObject->GetComponents())
        {
            if (!LinkDependency(handle, services, c))
            {
                return false;
            }
        }
        return true;
    }

    void UnHookServices(GameObjectHandle& handle, const Services& services, Component* component)
    {
        GameObject* gameObject = handle.Get();
        const MetaClass* metaClass = component->GetMetaClass();
        const u32 numDeps = metaClass->GetNumDependencies();
        for (u32 i=0; i < numDeps; ++i)
        {
            const MetaDependency* metaDependency = metaClass->GetDependencyAtIndex(i);
            const char* depTag = metaDependency->GetTag();
            if (strcmp(depTag, "Service") == 0)
            {
                // Look up the service and unsubscribe the gameObject
                const char* depName = metaDependency->GetName();
                Service* service = LookUpService(services, depName);
                if (service != nullptr && gameObject->HasService(service->GetID()))
                {
                    service->UnSubscribe(handle);
                }
            }
        }
    }
}

GameObjectFactory::GameObjectFactory(GameObjectPool& gameObjectPool)
    : mGameObjectPool(gameObjectPool)
{
}

//----------------------------------------------------------------------------------------------------

GameObjectFactory::~GameObjectFactory()
{
}

//----------------------------------------------------------------------------------------------------

void GameObjectFactory::Initialize(Services& services, GameWorld& world)
{
    mpWorld = &world;
    mServices = services;
}

//----------------------------------------------------------------------------------------------------

void GameObjectFactory::Terminate()
{
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameObjectFactory::Create(const char* templateFile)
{
    GameObjectHandle handle; // Init to invalid

    // Read the file into a stream
    std::ifstream data(templateFile);
    ASSERT(data.good(), "[GameObjectFactory] Error loading template file: %s", templateFile);
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(data, root))
    {
        data.close();
        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
        return handle;
    }
    data.close();

    // We have data, so allocate space for the object
    handle = mGameObjectPool.Allocate();
    GameObject* gameObject = handle.Get();
    gameObject->spWorld = mpWorld;

    // Load components
    Json::Value jcomponents = root["Components"];
    const u32 numComponents = jcomponents.size();
    for (u32 i=0; i < numComponents; ++i)
    {
        Json::Value& jcomponent = jcomponents[i];

        // Get the meta class by name and create an instance of the component
        std::string componentName = jcomponent.get("Name", "").asString();
        const MetaClass* metaClass = MetaDB::GetMetaClass(componentName.c_str());
        ASSERT(metaClass != nullptr, "[GameObjectFactory] Could not get MetaClass for component with name: %s", componentName);
        void* instance = metaClass->Create();

        Json::Value properties = jcomponent.get("Properties", "");
        for (Json::Value& p : properties)
        {
            std::string fieldName = p.get("Name", "").asString();
            const MetaField* metaField = metaClass->FindField(fieldName.c_str());
            ASSERT(metaField != nullptr, "[GameObjectFactory] Could not find field with name: %s", fieldName);

            // Deserialize the data into the field.
            // To keep this generic, the data for each field is held in an array
            void* member = (u8*)instance + metaField->GetOffset();
            metaField->GetType()->Deserialize(p.get("Data", ""), member);
        }
        // Add the component to the gameObject
        Component* component = static_cast<Component*>(instance);
        gameObject->AddComponent(component);
    }

    // Subscribe to services
    // This must be done in a second loop because some services may require a component that hasn't been loaded yet.
    VERIFY(LinkDependencies(handle, mServices), "[GameObjectFactory] Error: Could not create GameObject from file: %s due to missing dependencies", templateFile);

    return handle;
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameObjectFactory::Create(SerialReader& reader)
{
    GameObjectHandle handle = mGameObjectPool.Allocate();
    GameObject* gameObject = handle.Get();
    // TODO: 
    if (gameObject->Deserialize(reader))
    {
        gameObject->spWorld = mpWorld;
        LinkDependencies(handle, mServices);
        return handle;
    }
    return GameObjectHandle(); // invalid
}

//----------------------------------------------------------------------------------------------------

bool GameObjectFactory::Destroy(GameObjectHandle handle)
{
    if (!handle.IsValid())
        return false;

    if (OnDestroyGameObject(handle))
    {
        GameObject* gameObject = handle.Get();
        GameObject::Components::iterator it = gameObject->mComponents.begin();
        for (it; it != gameObject->mComponents.end(); ++it)
        {
            Component* component = *it;
            const MetaClass* metaClass = component->GetMetaClass();

            // Unsubscribe from any services
            UnHookServices(handle, mServices, component);

            // Delete the component
            metaClass->Destroy(component);
        }
        gameObject->mComponents.clear();
        mGameObjectPool.Free(handle);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool GameObjectFactory::AddComponent(GameObjectHandle handle, const char* componentMetaName)
{
    const MetaClass* metaClass = MetaDB::GetMetaClass(componentMetaName);
    if (metaClass != nullptr)
    {
        // TODO: specify in metadata whether multiple of a component can exist
        Component* component = static_cast<Component*>(metaClass->Create());
        if (component == nullptr)
        {
            return false;
        }
        
        // Add the component to the gameObject
        GameObject* gameObject = handle.Get();
        gameObject->AddComponent(component);

        // For now it doesn't matter if the service dependencies aren't met as we can just not subscribe.
        // In the future this may need to change.
        LinkDependency(handle, mServices, component);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

bool GameObjectFactory::RemoveComponent(GameObjectHandle handle, const char* componentMetaName)
{
    GameObject* gameObject = handle.Get();
    if (gameObject == nullptr)
    {
        return false;
    }

    GameObject::Components::iterator it = gameObject->mComponents.begin();
    for (it; it != gameObject->mComponents.end(); ++it)
    {
        Component* component = *it;

        // Look up component by name
        const MetaClass* metaClass = component->GetMetaClass();
        if (strcmp(componentMetaName, metaClass->GetName()) == 0)
        {
            // Unsubscribe
            UnHookServices(handle, mServices, component);

            // Remove and Delete the component
            gameObject->mComponents.erase(it);
            metaClass->Destroy(component);
            return true;
        }
    }
    return false;
}