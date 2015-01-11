#ifndef INCLUDED_ENGINE_GRAPHICSSYSTEM_H
#define INCLUDED_ENGINE_GRAPHICSSYSTEM_H

//====================================================================================================
// Filename:	GraphicsSystem.h
// Created by:	Peter Chan
// Description:	Class for the graphics system.
//====================================================================================================

#include "Color.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

#include "Color.h"

class GraphicsSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

<<<<<<< HEAD
	void BeginRender(const Color& clearColor = Color::Blue());
=======
	void BeginRender(const Color& clearColor = Color::Black());
>>>>>>> c02e68c600a3046aca295e57ef4fcddf0f74138d
	void EndRender();

	void ToggleFullscreen();
	void ResetRenderTarget();
	void ResetViewport();

	void EnableDepthTesting(bool enable);
	
	u32 GetWidth() const;
	u32 GetHeight() const;

	ID3D11Device* GetDevice()			{ return mpD3DDevice; }
	ID3D11DeviceContext* GetContext()	{ return mpImmediateContext; }
	
private:
	NONCOPYABLE(GraphicsSystem);

	ID3D11Device* mpD3DDevice;
	ID3D11DeviceContext* mpImmediateContext;

	IDXGISwapChain* mpSwapChain;
	ID3D11RenderTargetView* mpRenderTargetView;

	ID3D11Texture2D* mpDepthStencilBuffer;
	ID3D11DepthStencilView* mpDepthStencilView;
	ID3D11DepthStencilState* mpDisableDepthStencil;

	D3D_DRIVER_TYPE mDriverType;
	D3D_FEATURE_LEVEL mFeatureLevel;

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;

	D3D11_VIEWPORT mViewport;
	
	bool mFullscreen;
	bool mInitialized;
};

#endif // #ifndef INCLUDED_ENGINE_GRAPHICSSYSTEM_H