#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "MemoryPool.h"
#include "IO.h"
#include "RenderService.h"

#include <json/json.h>
#include <fstream>

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

    void LinkDependencies(GameObjectHandle& handle, const Services& services)
    {
        GameObject* gameObject = handle.Get();
        for (Component* c : gameObject->GetComponents())
        {
            const MetaClass* metaClass = c->GetMetaClass();
            const u32 numDependencies = metaClass->GetNumDependencies();
            const MetaDependency* dependencies = metaClass->GetDependencyList();
            for (u32 j=0; j < numDependencies; ++j)
            {
                // Compare the dependency name with the service name
                const char* depName = dependencies[j].GetName();
                Service* service = LookUpService(services, depName);
                if (service != nullptr)
                {
                    service->Subscribe(handle);
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

void GameObjectFactory::Initialize(Services& services)
{
    mServices = std::move(services);
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
        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
        return handle;
    }

    // We have data, so allocate space for the object
    handle = mGameObjectPool.Allocate();
    GameObject* gameObject = handle.Get();

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
    LinkDependencies(handle, mServices);

    return handle;
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameObjectFactory::Create(SerialReader& reader)
{
    GameObjectHandle handle = mGameObjectPool.Allocate();
    GameObject* gameObject = handle.Get();
    if (gameObject->Deserialize(reader))
    {
        LinkDependencies(handle, mServices);
        return handle;
    }
    return GameObjectHandle(); // invalid
}