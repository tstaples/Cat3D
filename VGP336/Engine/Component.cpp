#include "Precompiled.h"
#include "Component.h"

#include "GameObject.h"

META_CLASS_BEGIN(Component)
META_CLASS_END

Component::Component(GameObject* gameObject)
    : mpGameObject(gameObject)
{
}

//----------------------------------------------------------------------------------------------------

Component::~Component()
{
}