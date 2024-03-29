#include "Precompiled.h"
#include "MeshRendererComponent.h"
#include "RenderService.h"

META_CLASS_BEGIN(MeshRendererComponent)
META_FIELD_BEGIN
    META_FIELD(mTexturePath, "TexturePath")
META_FIELD_END
META_DEPENDENCIES_BEGIN
    META_DEPENDENCY("RenderService", "Service")
META_DEPENDENCIES_END
META_CLASS_END

MeshRendererComponent::MeshRendererComponent()
{
}

//----------------------------------------------------------------------------------------------------

MeshRendererComponent::~MeshRendererComponent()
{
    // TODO: have this managed by some system
    mTexture.Terminate();
}