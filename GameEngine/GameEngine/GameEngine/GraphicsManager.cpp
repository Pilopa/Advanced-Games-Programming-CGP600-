#pragma once

#include <memory>
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "LightingManager.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Debug.h"
#include "GameManager.h"
#include "Scene.h"
#include "GameObject.h"

const float GraphicsManager::DEFAULT_BACKGROUND_COLOR[] = { 0.0F, 0.0F, 1.0F, 1.0F };

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

	// Setup Depth Buffer State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &zBufferStateDisabled);
	if (FAILED(result))
		throw GraphicsManagerCreationException();

	deviceContext->OMSetDepthStencilState(NULL, 1);

	// Setup the raster description which will determine how and what polygons will be drawn by default.
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterStateDefault);
	if (FAILED(result))
		throw GraphicsManagerCreationException();

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterStateDefault);

	// Setup raster state without backface culling
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterStateNoBackfaceCulling);
	if (FAILED(result))
		throw GraphicsManagerCreationException();

	// Setup blend state for additive lighting calculations
	D3D11_BLEND_DESC1 BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC1));
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	device->QueryInterface(__uuidof(ID3D11Device1), (void **) &pDevice);
	pDevice->CreateBlendState1(&BlendState, &additiveBlendState);

	// Setup the rest

	if (FAILED(setupBackbuffer(width, height, sd.SampleDesc.Count)))
		throw GraphicsManagerCreationException();

	if (FAILED(setupDeferredShadingResources(width, height, sd.SampleDesc.Count)))
		throw GraphicsManagerCreationException();

	if (FAILED(setupRenderTargetView()))
		throw GraphicsManagerCreationException();

	if (FAILED(setupViewport(width, height)))
		throw GraphicsManagerCreationException();

	if (FAILED(setupRenderQuad(width, height)))
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

HRESULT GraphicsManager::setupRenderQuad(UINT width, UINT height)
{

	if (renderQuad) {
		renderQuad->release();
		delete renderQuad;
		renderQuad = nullptr;
	}
	int vertexCount = RENDER_QUAD_VERTEX_COUNT;
	std::vector<Vertex> vertices(vertexCount, Vertex());
	std::vector<UINT> indices(vertexCount, 0);
	int i;

	// Load the vertex array with data.
	// First triangle.
	vertices[0].Pos = { -1.0f, 1.0f, 0.0f };  // Top left.
	vertices[0].TexCoord = { 0.0f, 0.0f };

	vertices[1].Pos = { 1.0f, -1.0f, 0.0f };  // Bottom right.
	vertices[1].TexCoord = { 1.0f, 1.0f };

	vertices[2].Pos = { -1.0f, -1.0, 0.0f };  // Bottom left.
	vertices[2].TexCoord = { 0.0f, 1.0f };

	// Second triangle.
	vertices[3].Pos = { -1.0f, 1.0f, 0.0f };  // Top left.
	vertices[3].TexCoord = { 0.0f, 0.0f };

	vertices[4].Pos = { 1.0f, 1.0f, 0.0f };  // Top right.
	vertices[4].TexCoord = { 1.0f, 0.0f };

	vertices[5].Pos = { 1.0f, -1.0f, 0.0f };  // Bottom right.
	vertices[5].TexCoord = { 1.0f, 1.0f };

	// Load the index array with data.
	for (i = 0; i<vertexCount; i++)
	{
		indices[i] = i;
	}

	renderQuad = new Mesh(vertices, indices, this);

	return S_OK;
}

HRESULT GraphicsManager::setupDeferredShadingResources(UINT width, UINT height, UINT sampleCount)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	int i;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (i = 0; i<BUFFER_COUNT; i++)
	{

		// Release previously stored texture
		if (deferredRenderTargetTextures[i] != nullptr) {
			deferredRenderTargetTextures[i]->Release();
			deferredRenderTargetTextures[i] = nullptr;
		}

		// Create new texture
		result = device->CreateTexture2D(&textureDesc, NULL, &deferredRenderTargetTextures[i]);
		if (FAILED(result))
		{
			return result;
		}
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(deferredRenderTargetTextures[i], NULL, &deferredRenderTargetViews[i]);
		if (FAILED(result))
		{
			return result;
		}
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Attempt to create shader resource views
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		// Release previously stored resource views
		if (deferredShaderResources[i] != nullptr) {
			deferredShaderResources[i]->Release();
			deferredShaderResources[i] = nullptr;
		}

		result = device->CreateShaderResourceView(deferredRenderTargetTextures[i], &shaderResourceViewDesc, &deferredShaderResources[i]);
		if (FAILED(result))
		{
			return result;
		}
	}

	return S_OK;

}

HRESULT GraphicsManager::createZBuffer(UINT width, UINT height, UINT sampleCount)
{
	HRESULT result = S_OK;

	// Release previously present zbuffers
	if (zBuffer) {
		zBuffer->Release();
		zBuffer = nullptr;
	}

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
		return result;

	// Create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &zBuffer);
	pZBufferTexture->Release();

	return result;
}

HRESULT GraphicsManager::enableZBuffer()
{
	deviceContext->OMSetDepthStencilState(NULL, 1);
	return S_OK;
}

HRESULT GraphicsManager::disableZBuffer()
{
	deviceContext->OMSetDepthStencilState(zBufferStateDisabled, 1);
	return S_OK;
}

HRESULT GraphicsManager::renderSceneToTexture()
{
	// Set the render targets
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, deferredRenderTargetViews, zBuffer);

	// Clear the render targets
	float zeroValues[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Clear color buffer
	deviceContext->ClearRenderTargetView(deferredRenderTargetViews[0], DEFAULT_BACKGROUND_COLOR);
	
	// Clear normal buffer
	deviceContext->ClearRenderTargetView(deferredRenderTargetViews[1], zeroValues);

	// Clear position buffer
	deviceContext->ClearRenderTargetView(deferredRenderTargetViews[2], zeroValues);

	// Clear the depth buffer
	deviceContext->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH, 1.0F, 0);

	// Render Skysphere
	renderSkybox();

	// Call renderers to draw to texture
	for (std::set<Renderer*>::const_iterator iterator = renderers.begin(), end = renderers.end(); iterator != end; ++iterator) {
		((Renderer*)*iterator)->draw();
	}

	// reset the render target view
	deviceContext->OMSetRenderTargets(1, &backBufferRenderTargetView, zBuffer);

	// Reset Shader resources
	ID3D11ShaderResourceView* pSRVNull[BUFFER_COUNT];
	for (int i = 0; i < BUFFER_COUNT; i++) {
		pSRVNull[i] = NULL;
	}
	deviceContext->PSSetShaderResources(0, 2, pSRVNull);

	return S_OK;
}

HRESULT GraphicsManager::renderSkybox()
{
	Scene* scene = GameManager::instance()->getScene();
	Renderer* skybox = scene->getSkybox();

	if (skybox != nullptr) {
		disableZBuffer();
		disableBackfaceCulling();

		skybox->draw();

		enableZBuffer();
		enableBackfaceCulling();

		return S_OK;
	}
	else return S_FALSE;
	
}

HRESULT GraphicsManager::disableBackfaceCulling()
{
	deviceContext->RSSetState(rasterStateNoBackfaceCulling);
	return S_OK;
}

HRESULT GraphicsManager::enableBackfaceCulling()
{
	deviceContext->RSSetState(rasterStateDefault);
	return S_OK;
}

void GraphicsManager::renderFrame()
{

	// Render Scene to separate Texture
	renderSceneToTexture();

	// Clear the back buffer
	deviceContext->ClearRenderTargetView(backBufferRenderTargetView, DEFAULT_BACKGROUND_COLOR);

	// Clear the depth buffer
	deviceContext->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH, 1.0F, 0);

	// Disable ZBuffer for post-processing
	disableZBuffer();

	// Put the quad to render in the graphics pipeline
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* pVB = renderQuad->getVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(renderQuad->getIndexBuffer(), renderQuad->getIndexFormat(), 0);
	deviceContext->IASetPrimitiveTopology(renderQuad->getPrimitiveTopology());

	// Post-processing: Render lighting
	LightingManager::instance()->render();

	// Render UI
	// TODO: implement ui rendering!

	// Re-enable zbuffer
	enableZBuffer();

	// Bring backbuffer to front
	if (vsync)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}
}

HRESULT GraphicsManager::resizeBuffers(UINT newWidth, UINT newHeight)
{
	if (!swapChain) return S_OK;

	// Clear render targets
	deviceContext->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's backbuffers.
	if (backBufferRenderTargetView != nullptr)
		backBufferRenderTargetView->Release();

	// Release all outstanding references to the swap chain's deffered render targets
	if (deferredRenderTargetViews != nullptr)
		for (int i = 0; i<BUFFER_COUNT; i++)
		{
			if (deferredRenderTargetViews[i] != nullptr)
				deferredRenderTargetViews[i]->Release();
		}

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

	hr = setupDeferredShadingResources(newWidth, newHeight, 1); // TODO: sample size should not 1 be
	if (FAILED(hr)) 
		return hr;

	hr = setupRenderTargetView();
	if (FAILED(hr))
		return hr;

	hr = setupViewport(newWidth, newHeight);
	if (FAILED(hr))
		return hr;

	hr = setupRenderQuad(newWidth, newHeight);
	if (FAILED(hr))
		return hr;

	return S_OK;
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

ID3D11ShaderResourceView * GraphicsManager::getDeferredShaderResourceView(int index)
{
	return deferredShaderResources[index];
}

ID3D11BlendState1 * GraphicsManager::getAdditiveBlendState()
{
	return additiveBlendState;
}

void GraphicsManager::shutdown()
{
	for (int i = 0; i<BUFFER_COUNT; i++)
		if (deferredRenderTargetViews[i])
			deferredRenderTargetViews[i]->Release();

	if (rasterStateDefault) rasterStateDefault->Release();
	if (zBufferStateDisabled) zBufferStateDisabled->Release();
	if (zBuffer) zBuffer->Release();
	if (backBufferRenderTargetView) backBufferRenderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (device) device->Release();
}

bool GraphicsManager::isInitialized()
{
	return initialized;
}
