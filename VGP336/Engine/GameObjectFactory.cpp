#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "RenderService.h"
#include "ModelManager.h"

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
            componentNames.push_back(components[i].get("Type", "Model").asString());
        }
    }
    else
    {
        OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
    }



    ID gameObjectId = mpGameObjectRepository.Allocate();
    ID transformId = mpTransformRepository.Allocate();
    ID modelID = mpModelRepository.Allocate();
    
    GameObject& gameObject = mpGameObjectRepository.GetItem(gameObjectId);
    TransformComponent& transformComponent = mpTransformRepository.GetItem(transformId);
    ModelComponent& modelComponent = mpModelRepository.GetItem(modelID);

    Model* model = mpModelManager.GetResource(L"../Data/Stuff/soldier.catm");
    modelComponent.SetModel(model);
    
    gameObject.mID = gameObjectId;
    gameObject.AddComponent(transformId);
    gameObject.AddComponent(modelID);

    mpRenderService.Subscribe(gameObjectId);

    return gameObjectId;
}