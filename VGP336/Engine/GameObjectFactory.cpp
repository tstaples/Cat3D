#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "ModelManager.h"
#include "RenderService.h"

#include <json/json.h>
#include <fstream>

GameObjectFactory::GameObjectFactory(GameObjectRepository& gameObjectRepository,
                                     TransformRepository& transformRepository,
                                     ModelRepository& modelRepository,
                                     RenderService& renderService,
                                     ModelManager& modelManager)
    : mpGameObjectRepository(gameObjectRepository)
    , mpTransformRepository(transformRepository)
    , mpModelRepository(modelRepository)
    , mpRenderService(renderService)
    , mpModelManager(modelManager)
{
}

//----------------------------------------------------------------------------------------------------

GameObjectFactory::~GameObjectFactory()
{
}

//----------------------------------------------------------------------------------------------------

// TODO: Package all object data as binary during build.
ID GameObjectFactory::Create(const char* templateName, const Math::Vector3& startPosition)
{
    std::vector<std::string> componentNames;
    std::ifstream data("../Data/GameObjects/Soldier.json");

    Json::Value root;
    Json::Reader reader;
    if (reader.parse(data, root))
    {
        Json::Value components = root["Components"];
        for (u32 i=0; i < components.size(); ++i)
        {
            std::string componentTypeStr = components[i].get("Type", "Model").asString();
            Meta::Type componentType = Meta::GetEnumValue(componentTypeStr.c_str());
        }
    }
    else
    {
        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
    }

    // get all components
    //  - compare value from type key to get enum value
    //  - switch on enum and create component of that type
    //  - GameObjectJSON::Load(Component* component)
    //      - component->Load(mReader

    ID gameObjectId = mpGameObjectRepository.Allocate();
    ID transformId = mpTransformRepository.Allocate();
    ID modelID = mpModelRepository.Allocate();
    
    GameObject& gameObject = mpGameObjectRepository.GetItem(gameObjectId);
    TransformComponent& transformComponent = mpTransformRepository.GetItem(transformId);
    ModelComponent& modelComponent = mpModelRepository.GetItem(modelID);

    Model* model = mpModelManager.GetResource(L"../Data/Stuff/soldierv2.catm");
    modelComponent.SetModel(model);
    
    gameObject.mID = gameObjectId;
    gameObject.AddComponent(transformId);
    gameObject.AddComponent(modelID);

    mpRenderService.Subscribe(gameObjectId);

    return gameObjectId;
}