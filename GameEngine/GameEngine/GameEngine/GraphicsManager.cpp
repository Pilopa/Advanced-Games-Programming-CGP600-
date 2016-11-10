#pragma once

#include <memory>
#include "GraphicsManager.h"
#include "ApplicationManager.h"

const float GraphicsManager::DEFAULT_BACKGROUND_COLOR[] = { 1.0F, 1.0F, 1.0F, 1.0F };

GraphicsManager* GraphicsManager::s_instance = nullptr;
bool GraphicsManager::initialized = false;

GraphicsManager * GraphicsManager::instance()
{
	if (!s_instance)
		s_instance = new GraphicsManager();

	return s_instance;
}

GraphicsManager::GraphicsManager()
{
	// Declare result variable
	HRESULT result = S_OK;

	// Declare device flag variable
	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	// Define the possible drivers to try
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0
								  //functionality on hardware that doesn't support it
								  D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
								  D3D_DRIVER_TYPE_REFERENCE,
	};

	// Calculate the number of drivers in the array above
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// Define the required/supported DirectX versions
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Calculate target bounds
	RECT bounds;
	ApplicationManager::instance()->getWindow()->getBounds(&bounds);
	UINT width = bounds.right - bounds.left;
	UINT height = bounds.bottom - bounds.top;

	// Build swap chain description
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = ApplicationManager::instance()->getWindow()->getWinAPIWindowInstance();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	// Attempt to use the defined drivers to create device and swapchain
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		result = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain,
			&device, &featureLevel, &deviceContext);

		if (SUCCEEDED(result))
			break;
	}

	// Check if the creation was successful
	if (FAILED(result))
		throw GraphicsManagerCreationException();

	if (FAILED(setupBackbuffer(width, height, sd.SampleDesc.Count)))
		throw GraphicsManagerCreationException();

	if (FAILED(setupRenderTargetView()))
		throw GraphicsManagerCreationException();

	if (FAILED(setupViewport(width, height)))
		throw GraphicsManagerCreationException();

	initialized = true;
}

HRESULT GraphicsManager::setupBackbuffer(UINT width, UINT height, UINT sampleCount)
{
	// Declare result variable
	HRESULT result = S_OK;

	// Create Z-Buffer
	result = createZBuffer(width, height, sampleCount);
	if (FAILED(result))
		return result;

	return result;
}

HRESULT GraphicsManager::setupRenderTargetView()
{
	// Declare result variable
	HRESULT result = S_OK;

	// Get pointer to back buffer texture
	ID3D11Texture2D *pBackBufferTexture;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	// Check for errors
	if (FAILED(result)) return result;

	// Use the back buffer texture pointer to create the render target view
	result = device->CreateRenderTargetView(pBackBufferTexture, NULL,
		&backBufferRenderTargetView);

	// Free temporary variable memory
	pBackBufferTexture->Release();

	// Check for errors
	if (FAILED(result)) return result;

	// Set the render target view
	deviceContext->OMSetRenderTargets(1, &backBufferRenderTargetView, zBuffer);

	return result;
}

HRESULT GraphicsManager::setupViewport(UINT width, UINT height)
{
	// Declare result variable
	HRESULT result = S_OK;
	
	// Set the viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float) width;
	viewport.Height = (float) (height == 0 ? DEFAULT_HEIGHT_VALUE : height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);

	return result;
}

HRESULT GraphicsManager::createZBuffer(UINT width, UINT height, UINT sampleCount)
{
	HRESULT result = S_OK;

	// Create Z-Buffer
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height == 0 ? DEFAULT_HEIGHT_VALUE : height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sampleCount;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	result = device->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	// Check if the creation was successful
	if (FAILED(result))
		throw result;

	// Create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &zBuffer);
	pZBufferTexture->Release();
}

void GraphicsManager::renderFrame()
{
	// Clear the back buffer - choose a colour you like
	deviceContext->ClearRenderTargetView(backBufferRenderTargetView, DEFAULT_BACKGROUND_COLOR);

	// Clear ZBuffer
	deviceContext->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);

	// Call renderers to draw
	for (std::set<Renderer*>::const_iterator iterator = renderers.begin(), end = renderers.end(); iterator != end; ++iterator) {
		((Renderer*)*iterator)->draw();
	}

	// Bring backbuffer to front
	swapChain->Present(0, 0);
}

HRESULT GraphicsManager::resizeBuffers(UINT newWidth, UINT newHeight)
{
	if (!swapChain) return S_OK;

	// Clear render targets
	deviceContext->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's backbuffers.
	if (backBufferRenderTargetView != nullptr)
		backBufferRenderTargetView->Release();

	// Delcare result variable
	HRESULT hr = S_OK;

	// Preserve the existing buffer count and format.
	// Automatically choose the width and height to match the client rect for HWNDs.
	hr = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr))
		return hr;

	hr = setupBackbuffer(newWidth, newHeight, 1); // TODO: sample size should not 1 be
	if (FAILED(hr))
		return hr;

	hr = setupRenderTargetView();
	if (FAILED(hr))
		return hr;

	hr = setupViewport(newWidth, newHeight);
	if (FAILED(hr))
		return hr;

}

void GraphicsManager::registerRenderer(Renderer * renderer)
{
	renderers.insert(renderer);
}

void GraphicsManager::deregisterRenderer(Renderer * renderer)
{
	renderers.erase(renderer);
}

ID3D11Device * GraphicsManager::getDevice()
{
	return device;
}

ID3D11DeviceContext * GraphicsManager::getDeviceContext()
{
	return deviceContext;
}

bool GraphicsManager::isInitialized()
{
	return initialized;
}
