#pragma once

#include <set>
#include <d3d11.h>
#include "GameObject.h"
#include "Renderer.h"

class GraphicsManager {

	public:
		void renderFrame();
		void registerRenderer(Renderer* renderer);
		void deregisterRenderer(Renderer* renderer);
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();

		static GraphicsManager* instance();

	private:

		GraphicsManager();
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		IDXGISwapChain* swapChain = nullptr;
		ID3D11DepthStencilView* zBuffer = nullptr;
		std::set<Renderer*> renderers;

		static GraphicsManager* s_instance;
};