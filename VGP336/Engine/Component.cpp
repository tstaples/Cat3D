#include "Precompiled.h"
#include "Component.h"

#include "GameObject.h"
#include "GameWorld.h"

META_CLASS_BEGIN(Component)
META_CLASS_END

Component::Component()
    : mpGameObject(nullptr)
    , mIsDirty(false)
{
}

//----------------------------------------------------------------------------------------------------

Component::~Component()
{
}

//----------------------------------------------------------------------------------------------------

const GameWorld* Component::GetWorld() const
{
    return GetObj()->GetWorld();
}