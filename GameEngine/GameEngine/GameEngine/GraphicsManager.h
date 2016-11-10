#pragma once

#include <set>
#include <d3d11.h>
#include <DirectXMath.h>
#include "GameObject.h"
#include "Renderer.h"

#define DEFAULT_HEIGHT_VALUE 8

class GraphicsManager {

	public:
		void renderFrame();
		HRESULT resizeBuffers(UINT newWidth, UINT newHeight);
		void registerRenderer(Renderer* renderer);
		void deregisterRenderer(Renderer* renderer);
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();

		static bool isInitialized();
		static GraphicsManager* instance();
		static const float DEFAULT_BACKGROUND_COLOR[];

	private:

		GraphicsManager();
		HRESULT setupBackbuffer(UINT width, UINT height, UINT sampleCount);
		HRESULT setupRenderTargetView();
		HRESULT setupViewport(UINT width, UINT height);
		HRESULT createZBuffer(UINT width, UINT height, UINT sampleCount);

		ID3D11RenderTargetView* backBufferRenderTargetView = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		IDXGISwapChain* swapChain = nullptr;
		ID3D11DepthStencilView* zBuffer = nullptr;
		std::set<Renderer*> renderers;

		static bool initialized;
		static GraphicsManager* s_instance;
};