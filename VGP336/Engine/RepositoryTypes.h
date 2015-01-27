#ifndef INCLUDED_ENGINE_REPOSITORYTYPES_H
#define INCLUDED_ENGINE_REPOSITORYTYPES_H

//====================================================================================================
// Filename:	RepositoryTypes.h
// Created by:	Tyler Staples
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Repository.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

//====================================================================================================
// Typedefs
//====================================================================================================

typedef Repository<GameObject>          GameObjectRepository;
typedef Repository<TransformComponent>  TransformRepository;
typedef Repository<ModelComponent>      ModelRepository;

#endif // #ifndef INCLUDED_ENGINE_REPOSITORYTYPES_H