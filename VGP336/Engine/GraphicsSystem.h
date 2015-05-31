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

    void BindWindow(HWND window);
    void Finalize();

	void BeginRender(u32 windowIndex, const Color& clearColor = Color::Black());
	void EndRender();

	void ToggleFullscreen();
	void ResetRenderTarget();
	void ResetViewport();
    void Resize(HWND window, u32& width, u32& height);

	void EnableDepthTesting(bool enable);
	
	u32 GetWidth() const;
	u32 GetHeight() const;

	ID3D11Device* GetDevice()			{ return mpD3DDevice; }
	ID3D11DeviceContext* GetContext()	{ return mpImmediateContext; }
	
private:
    bool CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGISwapChain*& pSwapChain);

private:
	NONCOPYABLE(GraphicsSystem);
    typedef std::vector<ID3D11RenderTargetView*> RenderTargetViews;
    typedef std::vector<IDXGISwapChain*> SwapChains;

	ID3D11Device* mpD3DDevice;
	ID3D11DeviceContext* mpImmediateContext;

	IDXGISwapChain* mpSwapChain;
	ID3D11RenderTargetView* mpRenderTargetView;

    u32 mCurrentWindowIndex;
    SwapChains mSwapChains;
    RenderTargetViews mRenderTargetViews;
    ID3D11RenderTargetView** mpRenderTargetViews; // array

	ID3D11Texture2D* mpDepthStencilBuffer;
	ID3D11DepthStencilView* mpDepthStencilView;
	ID3D11DepthStencilState* mpDisableDepthStencil;

	D3D_DRIVER_TYPE mDriverType;
	D3D_FEATURE_LEVEL mFeatureLevel;

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;

	D3D11_VIEWPORT mViewport;
	
	bool mFullscreen;
	bool mInitialized;
    bool mFinalized;
};

#endif // #ifndef INCLUDED_ENGINE_GRAPHICSSYSTEM_H