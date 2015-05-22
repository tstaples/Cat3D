//====================================================================================================
// Filename:	Texture.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Texture.h"
#include "GraphicsSystem.h"
#include "IO.h"

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

Texture::Texture()
	: mpTexture(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------
Texture::~Texture()
{
	ASSERT(mpTexture == nullptr, "[Texture] Texture not released!");
}

//----------------------------------------------------------------------------------------------------

void Texture::Initialize(GraphicsSystem& gs, const wchar_t* pFilename)
{
    // Get the extension of the texture file to determine how it's loaded
    std::string ext = IO::GetExtension(pFilename);

    HRESULT hr;
    if (ext.compare("dds") == 0)
    {
    	hr = CreateDDSTextureFromFile(gs.GetDevice(), pFilename, nullptr, &mpTexture);
    }
    else
    {
        hr = CreateWICTextureFromFile(gs.GetDevice(), gs.GetContext(), pFilename, nullptr, &mpTexture);
    }
    // TODO: Output to error log
    ASSERT(hr == 0, "[Texture] Failed to load texture: %ls", pFilename);
}

//----------------------------------------------------------------------------------------------------

void Texture::Terminate()
{
	SafeRelease(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindVS(GraphicsSystem& gs, u32 index)
{
	gs.GetContext()->VSSetShaderResources(index, 1, &mpTexture);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindPS(GraphicsSystem& gs, u32 index)
{
	gs.GetContext()->PSSetShaderResources(index, 1, &mpTexture);
}