#include "Precompiled.h"
#include "MeshRendererComponent.h"

META_CLASS_BEGIN(MeshRendererComponent)
META_FIELD_BEGIN
    META_FIELD(mTexturePath, "TexturePath")
META_FIELD_END
META_CLASS_END

MeshRendererComponent::MeshRendererComponent()
{
    memset(mTexturePath, 0, MAX_PATH * sizeof(wchar_t));
}

//----------------------------------------------------------------------------------------------------

MeshRendererComponent::~MeshRendererComponent()
{
    // TODO: have this managed by some system
    mTexture.Terminate();
}