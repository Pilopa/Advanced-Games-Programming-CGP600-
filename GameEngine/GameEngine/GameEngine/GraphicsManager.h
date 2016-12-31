#pragma once

#include <set>
#include <d3d11.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "ClassDef.h"
#include "SpriteBatch.h"

#define DEFAULT_HEIGHT_VALUE 8
#define BUFFER_COUNT 3 // [color, normals, position]
#define RENDER_QUAD_VERTEX_COUNT 6

class GraphicsManager {

	public:
		void renderFrame();
		HRESULT resizeBuffers(UINT newWidth, UINT newHeight);
		void registerRenderer(Renderer* renderer);
		void deregisterRenderer(Renderer* renderer);
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		ID3D11ShaderResourceView* getDeferredShaderResourceView(int index);
		ID3D11BlendState1* getAdditiveBlendState();
		DirectX::SpriteBatch* getSpriteBatch();
		void shutdown();

		static bool isInitialized();
		static GraphicsManager* instance();
		static const float DEFAULT_BACKGROUND_COLOR[];

	private:
		GraphicsManager();
		HRESULT setupBackbuffer(UINT width, UINT height, UINT sampleCount);
		HRESULT setupRenderTargetView();
		HRESULT setupViewport(UINT width, UINT height);
		HRESULT setupRenderQuad(UINT width, UINT height);
		HRESULT setupDeferredShadingResources(UINT width, UINT height, UINT sampleCount);
		HRESULT createZBuffer(UINT width, UINT height, UINT sampleCount);
		HRESULT enableZBuffer();
		HRESULT disableZBuffer();
		HRESULT renderSceneToTexture();
		HRESULT renderSkybox();
		HRESULT disableBackfaceCulling();
		HRESULT enableBackfaceCulling();

		ID3D11Texture2D* deferredRenderTargetTextures[BUFFER_COUNT];
		ID3D11RenderTargetView* deferredRenderTargetViews[BUFFER_COUNT];
		ID3D11ShaderResourceView* deferredShaderResources[BUFFER_COUNT];

		DirectX::SpriteBatch* spriteBatch;
		Mesh* renderQuad = nullptr;
		ID3D11RenderTargetView* backBufferRenderTargetView = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11Device1* pDevice = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;
		IDXGISwapChain* swapChain = nullptr;
		ID3D11DepthStencilView* zBuffer = nullptr;
		ID3D11DepthStencilState* zBufferStateDisabled = nullptr;
		ID3D11RasterizerState* rasterStateDefault = nullptr;
		ID3D11RasterizerState* rasterStateNoBackfaceCulling = nullptr;
		ID3D11BlendState1* additiveBlendState;
		std::set<Renderer*> renderers;
		bool vsync = false;

		static bool initialized;
		static GraphicsManager* s_instance;
};