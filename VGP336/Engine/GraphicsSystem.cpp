//====================================================================================================
// Filename:	GraphicsSystem.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "GraphicsSystem.h"

//====================================================================================================
// Local Definitions
//====================================================================================================

namespace
{
	const D3D_DRIVER_TYPE kDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const UINT kNumDriverTypes = ARRAYSIZE(kDriverTypes);

	const D3D_FEATURE_LEVEL kFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const UINT kNumFeatureLevels = ARRAYSIZE(kFeatureLevels);

    void GetViewportSize(HWND hWnd, UINT& width, UINT& height)
    {
        RECT rc = { 0 };
	    GetClientRect(hWnd, &rc);
	    width = rc.right - rc.left;
	    height = rc.bottom - rc.top;
    }

    UINT GetCreateDeviceFlags()
    {
        UINT createDeviceFlags = 0;
    #ifdef _DEBUG
	    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
        return createDeviceFlags;
    }
}

struct WindowDataContainer
{
    IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;
    ID3D11DepthStencilView* pDepthStencilView;
    D3D11_VIEWPORT viewport;
    HWND hWnd;
    u32 width;
    u32 height;
};

//====================================================================================================
// Class Definitions
//====================================================================================================

GraphicsSystem::GraphicsSystem()
	: mpD3DDevice(nullptr)
	, mpImmediateContext(nullptr)
	, mpDepthStencilBuffer(nullptr)
	, mpDisableDepthStencil(nullptr)
    , mpCurrentDepthStencilView(nullptr)
    , mpCurrentSwapChain(nullptr)
    , mpCurrentRenderTargetView(nullptr)
	, mDriverType(D3D_DRIVER_TYPE_NULL)
	, mFeatureLevel(D3D_FEATURE_LEVEL_11_0)
	, mFullscreen(false)
	, mInitialized(false)
    , mCurrentWindowIndex(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------
GraphicsSystem::~GraphicsSystem()
{
	ASSERT(!mInitialized, "[GraphicsSystem] Terminate() must be called to clean up!");
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		LOG("[GraphicsSystem] System already initialized.");
		return;
	}
	LOG("[GraphicsSystem] Initializing...");

	// Now that we have a window that is displaying, we can continue to set up a Direct3D 11 device.
	// Setup is necessary if we are going to render any 3D scene. The first thing to do is to create
	// three objects: a device, an immediate context, and a swap chain. The immediate context is a
	// new object in Direct3D 11.

    CreateDevice(fullscreen);

    BindWindow(window);

    // Initialize 'current' pointers to the first window
    WindowDataContainer* windowData = mWindowDataContainers.front();
    mpCurrentSwapChain = windowData->pSwapChain;
    mpCurrentRenderTargetView = windowData->pRenderTargetView;
    mpCurrentDepthStencilView = windowData->pDepthStencilView;

    // Initialize the DX COM library
    // Enables DXTK to load non .dds textures
    CoInitialize(nullptr);

	// Set flag
	mInitialized = true;

	LOG("[GraphicsSystem] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::BindWindow(HWND window)
{
    WindowDataContainer* windowData = new WindowDataContainer();
    windowData->hWnd = window;
    GetViewportSize(window, windowData->width, windowData->height);

    // Create the swap chain
    CreateSwapChain(windowData);

    // The next thing we need to do is to create a render target view. A render target view is a type
	// of resource view in Direct3D 11. A resource view allows a resource to be bound to the graphics
	// pipeline at a specific stage. Think of resource views as typecast in C. A chunk of raw memory
	// in C can be cast to any data type. We can cast that chunk of memory to an array of integers,
	// an array of floats, a structure, an array of structures, and so on. The raw memory itself is
	// not very useful to us if we don't know its type. Direct3D 11 resource views act in a similar
	// way. For instance, a 2D texture, analogous to the raw memory chunk, is the raw underlying
	// resource. Once we have that resource we can create different resource views to bind that
	// texture to different stages in the graphics pipeline with different formats: as a render target
	// to which to render, as a depth stencil buffer that will receive depth information, or as a
	// texture resource. Where C typecasts allow a memory chunk to be used in a different manner, so
	// do Direct3D 11 resource views.

    // Create the backbuffer
    ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = windowData->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to access swap chain buffer.");

    // We need to create a render target view because we would like to bind the back buffer of our
	// swap chain as a render target. This enables Direct3D 11 to render onto it. We first call
	// GetBuffer() to get the back buffer object. Optionally, we can fill in a D3D11_RENDERTARGETVIEW_DESC
	// structure that describes the render target view to be created. This description is normally the
	// second parameter to CreateRenderTargetView. However, for these tutorials, the default render
	// target view will suffice. The default render target view can be obtained by passing NULL as the
	// second parameter. Once we have created the render target view, we can call OMSetRenderTargets() on
	// the immediate context to bind it to the pipeline. This ensures the output that the pipeline renders
	// gets written to the back buffer. The code to create and set the render target view is as follows:

    // Create the render target
    hr = mpD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &windowData->pRenderTargetView);
	SafeRelease(pBackBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create render target view.");

    // Create the depth stencil
    CreateDepthStencil(windowData);

    // Create the viewport
    CreateViewPort(windowData);

    // Increase the window count
    mWindowDataContainers.push_back(windowData);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		LOG("[GraphicsSystem] System already terminated.");
		return;
	}

	LOG("[GraphicsSystem] Terminating...");

 
	SafeRelease(mpDisableDepthStencil);
	SafeRelease(mpDepthStencilBuffer);

    // For some reason, releasing the pointers here causes c# to shit the bed
    // and not shutdown properly. So we'll let the memory be freed in the loop below.
	mpCurrentDepthStencilView = nullptr;
	mpCurrentRenderTargetView = nullptr;
	mpCurrentSwapChain = nullptr;

    const u32 mNumWindows = mWindowDataContainers.size();
    for (u32 i=0; i < mNumWindows; ++i)
    {
        WindowDataContainer* windowData = mWindowDataContainers[i];
        SafeRelease(windowData->pRenderTargetView);
        SafeRelease(windowData->pSwapChain);
        SafeRelease(windowData->pDepthStencilView);
        SafeDelete(windowData);
    }
    mWindowDataContainers.clear();

	SafeRelease(mpImmediateContext);
	SafeRelease(mpD3DDevice);

	// Set flag
	mInitialized = false;

	LOG("[GraphicsSystem] System terminated.");
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::BeginRender(u32 windowIndex, const Color& clearColor)
{
    ASSERT(windowIndex < mWindowDataContainers.size(), "[GraphicsSystem] Invalid window index");
    
    WindowDataContainer* windowData = mWindowDataContainers[windowIndex];
    mpCurrentSwapChain = windowData->pSwapChain;
    mpCurrentRenderTargetView = windowData->pRenderTargetView;
    mpCurrentDepthStencilView = windowData->pDepthStencilView;

    // Set the render target for this window
    mpImmediateContext->OMSetRenderTargets(1, &mpCurrentRenderTargetView, mpCurrentDepthStencilView);

    // Set the viewport
	mpImmediateContext->RSSetViewports(1, &windowData->viewport);

    mpImmediateContext->ClearRenderTargetView(mpCurrentRenderTargetView, clearColor.ToFloatArray());
	mpImmediateContext->ClearDepthStencilView(mpCurrentDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    mCurrentWindowIndex = windowIndex;
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::EndRender()
{
    mpCurrentSwapChain->Present(0, 0);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::ToggleFullscreen()
{
	mFullscreen = !mFullscreen;
	mpCurrentSwapChain->SetFullscreenState(mFullscreen, nullptr);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::ResetRenderTarget()
{
	ASSERT(mpImmediateContext != nullptr, "[GraphicsSystem] Failed to reset render target.");
	mpImmediateContext->OMSetRenderTargets(1, &mpCurrentRenderTargetView, mpCurrentDepthStencilView);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::ResetViewport()
{
	ASSERT(mpImmediateContext != nullptr, "[GraphicsSystem] Failed to reset viewport.");
	mpImmediateContext->RSSetViewports(1, &mWindowDataContainers[mCurrentWindowIndex]->viewport);
}

//----------------------------------------------------------------------------------------------------

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075(v=vs.85).aspx#Handling_Window_Resizing
void GraphicsSystem::Resize(HWND window, u32& width, u32& height)
{
    ASSERT(mWindowDataContainers[mCurrentWindowIndex]->hWnd == window, "[GraphicsSystem] Attempting to resize the incorrect window");
    ASSERT(mpCurrentSwapChain, "[GraphicsSystem] Failed to resize: Swap chain unitialized");

    // Release all outstanding references to the swap chain's buffers
    SafeRelease(mpDepthStencilBuffer);
    SafeRelease(mpCurrentRenderTargetView);

    UINT createDeviceFlags = GetCreateDeviceFlags();

    // Preserve exisiting buffer count and format
    // Automatically choose the width and height to match the client rect for HWND
    HRESULT hr = mpCurrentSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, createDeviceFlags);
    ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to resize buffers");

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpCurrentSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to get buffer");

    hr = mpD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpCurrentRenderTargetView);
    ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create render target view");
    SafeRelease(pBackBuffer);

    // Set the render target
    mpImmediateContext->OMSetRenderTargets(1, &mpCurrentRenderTargetView, nullptr);

    // Update the swap chain desc so it's aware of the new size
    mpCurrentSwapChain->GetDesc(&mSwapChainDesc);
    width = GetWidth();
    height = GetHeight();

    // Update the renderTargetView in the container
    WindowDataContainer* windowData = mWindowDataContainers[mCurrentWindowIndex];
    windowData->pRenderTargetView = mpCurrentRenderTargetView;

    // Update the viewport's dimensions
    windowData->viewport.Width = (FLOAT)width;
	windowData->viewport.Height = (FLOAT)height;
	mpImmediateContext->RSSetViewports(1, &windowData->viewport);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::EnableDepthTesting(bool enable)
{
	ASSERT(mpImmediateContext != nullptr, "[GraphicsSystem] Failed to set depth stencil state.");
	mpImmediateContext->OMSetDepthStencilState(enable ? nullptr : mpDisableDepthStencil, 0);
}

//----------------------------------------------------------------------------------------------------

u32 GraphicsSystem::GetWidth() const
{
	ASSERT(mpCurrentSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Width;
}

//----------------------------------------------------------------------------------------------------

u32 GraphicsSystem::GetHeight() const
{
	ASSERT(mpCurrentSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Height;
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::CreateDevice(bool fullscreen)
{
    // In Direct3D 11, the immediate context is used by the application to perform rendering onto a
	// buffer, and the device contains methods to create resources.

    UINT createDeviceFlags = GetCreateDeviceFlags();
	mFullscreen = fullscreen;

	HRESULT hr = S_FALSE;
	for (UINT driverTypeIndex = 0; driverTypeIndex < kNumDriverTypes; ++driverTypeIndex)
	{
		mDriverType = kDriverTypes[driverTypeIndex];
        hr = D3D11CreateDevice
        (
            nullptr,
            mDriverType,
            nullptr,
            createDeviceFlags,
            kFeatureLevels,
            kNumFeatureLevels,
            D3D11_SDK_VERSION,
            &mpD3DDevice,
			&mFeatureLevel,
			&mpImmediateContext
        );
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create device or swap chain.");
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::CreateSwapChain(WindowDataContainer* windowData)
{
    // The swap chain is responsible for taking the buffer to which the device renders, and displaying
	// the content on the actual monitor screen. The swap chain contains two or more buffers, mainly
	// the front and the back. These are textures to which the device renders in order to display on
	// the monitor. The front buffer is what is being presented currently to the user. This buffer is
	// read-only and cannot be modified. The back buffer is the render target to which the device will
	// draw. Once it finishes the drawing operation, the swap chain will present the backbuffer by
	// swapping the two buffers. The back buffer becomes the front buffer, and vice versa.

    // To create the swap chain, we fill out a DXGI_SWAPCHAIN_DESC structure that describes the swap
	// chain we are about to create. A few fields are worth mentioning. BackBufferUsage is a flag that
	// tells the application how the back buffer will be used. In this case, we want to render to the
	// back buffer, so we'll set BackBufferUsage to DXGI_USAGE_RENDER_TARGET_OUTPUT. The OutputWindow
	// field represents the window that the swap chain will use to present images on the screen.
	// SampleDesc is used to enable multi-sampling. Since this tutorial does not use multi-sampling,
	// SampleDesc's Count is set to 1 and Quality to 0 to have multi-sampling disabled.

    DXGI_SWAP_CHAIN_DESC descSwapChain;
	ZeroMemory(&descSwapChain, sizeof(descSwapChain));
	descSwapChain.BufferCount = 1;
	descSwapChain.BufferDesc.Width = windowData->width;
	descSwapChain.BufferDesc.Height = windowData->height;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.OutputWindow = windowData->hWnd;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.Windowed = !mFullscreen;

    // If the swap chain is going to be used by Direct3D 11, it must be created with the same factory that was used to create the Direct3D device
    // https://msdn.microsoft.com/en-us/library/windows/desktop/hh404556(v=vs.85).aspx

    // Get the device interface
    IDXGIDevice2* pDXGIDevice = nullptr;
    HRESULT hr = mpD3DDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
    ASSERT(SUCCEEDED(hr), "[GraphicSystem] Failed to get IDXGIDevice2");

    // Use the device to get the adapter
    IDXGIAdapter* pDXGIAdapter = nullptr;
    hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
    ASSERT(SUCCEEDED(hr), "[GraphicSystem] Failed to get IDXGIAdapter");

    // Get the factory interface from the adapter...
    IDXGIFactory2* pIDXGIFactory2 = nullptr;
    hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory2);
    ASSERT(SUCCEEDED(hr), "[GraphicSystem] Failed to get IDXGIFactory2");

    // Finally create the swap chain using the factory interface
    hr = pIDXGIFactory2->CreateSwapChain(mpD3DDevice, &descSwapChain, &windowData->pSwapChain);

    // Done with these for now
    SafeRelease(pDXGIDevice);
    SafeRelease(pDXGIAdapter);
    SafeRelease(pIDXGIFactory2);

    // Cache the description
    windowData->pSwapChain->GetDesc(&descSwapChain);
    
    ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create swap chain");
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::CreateDepthStencil(WindowDataContainer* windowData)
{
    // The depth buffer allows Direct3D to keep track of the depth of every pixel drawn to the screen.
	// The default behavior of the depth buffer in Direct3D 11 is to check every pixel being drawn to
	// the screen against the value stored in the depth buffer for that screen-space pixel. If the depth
	// of the pixel being rendered is less than or equal to the value already in the depth buffer, the
	// pixel is drawn and the value in the depth buffer is updated to the depth of the newly drawn
	// pixel. On the other hand, if the pixel being draw has a depth greater than the value already in
	// the depth buffer, the pixel is discarded and the depth value in the depth buffer remains unchanged.

	// The following code in the sample creates a depth buffer (a DepthStencil texture). It also creates
	// a DepthStencilView of the depth buffer so that Direct3D 11 knows to use it as a Depth Stencil texture.

    // Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = windowData->width;
	descDepth.Height = windowData->height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = mpD3DDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create depth stencil buffer.");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mpD3DDevice->CreateDepthStencilView(mpDepthStencilBuffer, &descDSV, &windowData->pDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create depth stencil view.");
	
	// Set the render target view and depth stencil view
	//mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);
		
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = false;
	mpD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mpDisableDepthStencil);
}

//----------------------------------------------------------------------------------------------------

void GraphicsSystem::CreateViewPort(WindowDataContainer* windowData)
{
    // The last thing we need to set up before Direct3D 11 can render is initialize the viewport. The
	// viewport maps clip space coordinates, where X and Y range from -1 to 1 and Z ranges from 0 to 1,
	// to render target space, sometimes known as pixel space. In Direct3D 9, if the application does
	// not set up a viewport, a default viewport is set up to be the same size as the render target.
	// In Direct3D 11, no viewport is set by default. Therefore, we must do so before we can see anything
	// on the screen. Since we would like to use the entire render target for the output, we set the top
	// left point to (0, 0) and width and height to be identical to the render target's size. To do this,
	// use the following code:

	// Setup the viewport
	windowData->viewport.Width = (FLOAT)windowData->width;
	windowData->viewport.Height = (FLOAT)windowData->height;
	windowData->viewport.MinDepth = 0.0f;
	windowData->viewport.MaxDepth = 1.0f;
	windowData->viewport.TopLeftX = 0;
	windowData->viewport.TopLeftY = 0;
}