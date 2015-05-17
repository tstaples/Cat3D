#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "MemoryPool.h"
#include "IO.h"

#include <json/json.h>
#include <fstream>

GameObjectFactory::GameObjectFactory(GameObjectPool& gameObjectPool)
    : mGameObjectPool(gameObjectPool)
{
}

//----------------------------------------------------------------------------------------------------

GameObjectFactory::~GameObjectFactory()
{
}

//----------------------------------------------------------------------------------------------------

GameObjectHandle GameObjectFactory::Create(const char* templateFile)
{
    GameObjectHandle handle = mGameObjectPool.Allocate();
    GameObject* gameObject = handle.Get();

    // TODO
    // while (going through JSON tree)...
    const MetaClass* metaClass = MetaDB::GetMetaClass("TransformComponent");
    void* instance = metaClass->Create();

    /*
        - Traverse JSON tree
        - loop through all components
            - loop through each component field
                JSON currentElement;
                MetaField* field = metaClass->FindField("componentName");
                void* member = (u8*)instance + field->GetOffset();
                metaField->GetMetaType()->Deserialize(member, currentElement);

                Deserialize will be a template function with specializations for each registered type.
                - TODO: overload for different serialization methods (json, binary etc.) this way it can be used to communicate with the editor.
    */

    //gameObject->AddComponent(component);

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