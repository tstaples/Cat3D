#ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_ENGINE_GAMEOBJECTFACTORY_H

//====================================================================================================
// Filename:	GameObjectFactory.h
// Created by:	Tyler Staples
// Description: Class for a GameObject Factory.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "RepositoryTypes.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class RenderService;
class ModelManager;

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObjectFactory
{
public:
    /* By taking references to these different systems and services, it enforces
       the proper creation order. 
       Ideally this would be simplified by a better meta-type system.
    */
    GameObjectFactory(GameObjectRepository& gameObjectRepository,
                      TransformRepository& transformRepository,
                      ModelRepository& modelRepository,
                      RenderService& renderService,
                      ModelManager& modelManager);
    ~GameObjectFactory();

    /*  Creates an object based on the details laid out in the template file.
    *   @param templateName: template file path.
    *   @oaram startPosition: position in the world the object will be placed. 
    *   returns: ID of created object. 
    */
    ID Create(const char* templateName, const Math::Vector3& startPosition);

private:
    GameObjectRepository& mpGameObjectRepository;

    // Component repositories
    TransformRepository& mpTransformRepository;
    ModelRepository& mpModelRepository;

    // Services
    RenderService& mpRenderService;

    ModelManager& mpModelManager;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H