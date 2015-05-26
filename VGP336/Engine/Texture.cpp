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

bool Texture::Initialize(GraphicsSystem& gs, const wchar_t* pFilename)
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
    
    // Load default 
    bool loaded = (hr == 0);
    if (!loaded)
    {
        const wchar_t* defaultTexture = L"../Engine/Images/checker.dds";
    	hr = CreateDDSTextureFromFile(gs.GetDevice(), defaultTexture, nullptr, &mpTexture);
        ASSERT(hr == 0, "[Texture] Failed to load default texture: %ls", defaultTexture);
    }

    // TODO: Output to error log
    //ASSERT(hr == 0, "[Texture] Failed to load texture: %ls", pFilename);
    return (hr == 0);
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