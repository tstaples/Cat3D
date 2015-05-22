#include "Precompiled.h"
#include "Component.h"

#include "GameObject.h"

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