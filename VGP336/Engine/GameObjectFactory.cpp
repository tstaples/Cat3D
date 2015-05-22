#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "MemoryPool.h"
#include "IO.h"
#include "RenderService.h"

#include <json/json.h>
#include <fstream>

GameObjectFactory::GameObjectFactory(GameObjectPool& gameObjectPool, u32 componentPoolCapacity)
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
    mServices = &services;
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
        // Not sure if this will work
        Component* component = static_cast<Component*>(instance);
        gameObject->AddComponent(component);
    }

    // Subscribe to services
    Json::Value jservices = root["Services"];
    for (Json::Value& jservice : jservices)
    {
        // Look up service by name
        std::string serviceName = jservice.get("Name", "").asString();
        for (auto service : *mServices)
        {
            if (serviceName.compare(service.first) == 0)
            {
                // Subscribe the gameObject to it
                service.second->Subscribe(handle);
            }
        }
    }
    return handle;
}

// TODO: Package all object data as binary during build.
//ID GameObjectFactory::Create(const char* templateName, const Math::Vector3& startPosition)
//{
//    ID gameObjectID;
//
//    std::ifstream data(templateName);
//    ASSERT(data.good(), "Error loading template file: %s", templateName);
//
//    Json::Value root;
//    Json::Reader reader;
//    if (reader.parse(data, root))
//    {
//        // Create a gameobject
//        gameObjectID = mpGameObjectRepository.Allocate();
//        GameObject& gameObject = mpGameObjectRepository.GetItem(gameObjectID);
//
//        // Go through all the components and add them to the game object
//        Json::Value components = root["Components"];
//        for (u32 i=0; i < components.size(); ++i)
//        {
//            std::string componentTypeStr = components[i].get("Type", "").asString();
//            Meta::Type componentType = Meta::GetEnumValue(componentTypeStr);
//            
//            ID componentID;
//            if (componentType == Meta::ModelComponentType)
//            {
//                componentID = mpModelRepository.Allocate();
//                ModelComponent& modelComponent = mpModelRepository.GetItem(componentID);
//
//                // Get the path for the model to load
//                Json::Value properties = components[i].get("Properties", "");
//                std::string filenameStr = properties.get("Filename", "").asString();
//                std::wstring filename = IO::CharToWChar(filenameStr);
//
//                Model* model = mpModelManager.GetResource(filename.c_str());
//                modelComponent.SetModel(model);
//            }
//            else if (componentType == Meta::TransformComponentType)
//            {
//                componentID = mpTransformRepository.Allocate();
//                //TransformComponent& TransformComponent = mpTransformRepository.GetItem(componentID);
//            }
//            else if (componentType == Meta::ColliderComponentType)
//            {
//            }
//
//            // Add the component
//            if (componentID.IsValid())
//            {
//                //gameObject.AddComponent(componentID);
//            }
//        }
//        
//        Json::Value services = root["Services"];
//        for (u32 i=0; i < services.size(); ++i)
//        {
//            std::string serviceTypeStr = services[i].get("Type", "").asString();
//            Meta::Type serviceType = Meta::GetEnumValue(serviceTypeStr);
//
//            switch (serviceType)
//            {
//            case Meta::RenderServiceType:
//                mpRenderService.Subscribe(gameObjectID);
//                break;
//            }
//        }
//    }
//    else
//    {
//        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
//    }
//
//    // get all components
//    //  - compare value from type key to get enum value
//    //  - switch on enum and create component of that type
//    //  - GameObjectJSON::Load(Component* component)
//    //      - component->Load(mReader
//    //
//    //gameObjectID = mpGameObjectRepository.Allocate();
//    //ID transformId = mpTransformRepository.Allocate();
//    //ID modelID = mpModelRepository.Allocate();
//    //
//    //GameObject& gameObject = mpGameObjectRepository.GetItem(gameObjectID);
//    //TransformComponent& transformComponent = mpTransformRepository.GetItem(transformId);
//    //ModelComponent& modelComponent = mpModelRepository.GetItem(modelID);
//    //
//    //Model* model = mpModelManager.GetResource(L"../Data/Stuff/soldierv2.catm");
//    //modelComponent.SetModel(model);
//    //
//    //gameObject.mID = gameObjectID;
//    //gameObject.AddComponent(transformId);
//    //gameObject.AddComponent(modelID);
//
//    //mpRenderService.Subscribe(gameObjectID);
//
//    return gameObjectID;
//}