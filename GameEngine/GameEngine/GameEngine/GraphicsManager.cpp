#pragma once

#include "GraphicsManager.h"
#include "ApplicationManager.h"

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

}

void GraphicsManager::renderFrame()
{
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

GraphicsManager* GraphicsManager::s_instance = nullptr;

GraphicsManager * GraphicsManager::instance()
{
	if (!s_instance)
		s_instance = new GraphicsManager();

	return s_instance;
}