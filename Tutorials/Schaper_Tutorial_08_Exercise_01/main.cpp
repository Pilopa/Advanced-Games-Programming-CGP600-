#include <Windowsx.h>
#include <d3d11.h>
#include <time.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <chrono>
#include <string>
#include "MyMath.h"
#include "Vertex.h"
#include "camera.h"
#include <memory>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

//////////////////////////////////////////////////////////////////////////////////////
// Marcos
//////////////////////////////////////////////////////////////////////////////////////
#define KeepInBounds(index, arr) ( ((index) < 0 ) ? (sizeof(arr) / sizeof(arr[0])) - 1 : (index) % (sizeof(arr) / sizeof(arr[0])) )
#define ZeroVariable(value) (ZeroMemory(&value, sizeof(value)))
#define SafeRelease(pIUnknown) if (pIUnknown) pIUnknown->Release()
#define OverflowAngle(degree) (degree < 0 ? 360 + degree : degree % 360)

//////////////////////////////////////////////////////////////////////////////////////
// Structs
//////////////////////////////////////////////////////////////////////////////////////

struct CONSTANT_BUFFER0 
{
	DirectX::XMMATRIX WorldViewProjection; // 64 Bytes ( 4x4 = 16 floats x 4 bytes )
	float RedAmount; // 4 Bytes
	float scale; // 4 Bytes
	DirectX::XMFLOAT2 packing_bytes1; // 2x4 bytes = 8 bytes
}; // = 80 bytes

//////////////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////////////

// Rename for each tutorial

const char g_TutorialName[100] = "Tutorial 05 Exercise 01\0";

// Window Specific Variables

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

// Transformation Matrices

DirectX::XMMATRIX WorldViewProjection, projection, view, rotation, translation, scale, world_transform;

// Direct 3D Variables

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pD3DDevice = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
const LPCWSTR g_pShaderFileName = L"shaders.hlsl";

// Buffers
ID3D11Buffer* g_pVertexBuffer;
ID3D11Buffer* g_pConstantBuffer0;
ID3D11DepthStencilView* g_pZBuffer;

// Shaders
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11InputLayout* g_pInputLayout;

// Other
Camera* g_pMainCamera;
short g_pCurrentVertexIndex = 0;
float g_clear_colour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
long red_timestamp = 0;
long blue_timestamp = 0;
long green_timestamp = 0;
float x = 10.0F;
float y = 10.0F;
float tx;
float ty;

// Defines a triangle of vertices, with each vertex having a distinct color
POS_COL_VERTEX g_pVertices[] =
{

	// back face
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

	// front face
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	// left face
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // green
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }, // yellow
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // green
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // blue
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }, // yellow

	// right face
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },

	// bottom face
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // blue
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }, // green
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }, // purple
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }, // green

	// top face
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }

};

// FPS Measurement, for more info about time measurement: http://stackoverflow.com/questions/14337278/precise-time-measurement
long long fpsTimer_t1, fpsTimer_t2; // timestamps
double fpsTimer_elapsedTime;
double FPS;
 
//////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
//////////////////////////////////////////////////////////////////////////////////////

HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
HRESULT InitialiseGraphics(void);
void RenderFrame(void);
void RandomizeBackgroundColor();
void RandomizeRed();
void RandomizeGreen();
void RandomizeBlue();
void ShutdownD3D();
void UpdateVertices();

//////////////////////////////////////////////////////////////////////////////////////
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Attempt to initialize the Window to be displayed,
	// exit the program with an error message on failure
	if(FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		OutputDebugStringA("Failed to create Window");
		return 0;
	}

	// Attempt to initialize the Direct3D render engine,
	// exit the program with an error message on failure
	if(FAILED(InitialiseD3D()))
	{
		OutputDebugStringA("Failed to create Device");
		return 0;
	}

	// 03-01
	if(FAILED(InitialiseGraphics()))
	{
		OutputDebugStringA("Failed to initialise graphics");
		return 0;
	}

	// Initialize random seed
	srand (time(NULL)); // Used by Background Color Randomization

	// Main message loop
	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			fpsTimer_t2 = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // timestamp in nanoseconds

			// Calculate time delta
			fpsTimer_elapsedTime = (fpsTimer_t2 - fpsTimer_t1) / 1000.0 / 1000.0 / 1000.0; // delta time in seconds

			// Update FPS Display once a second
			if (GetTickCount() % 1000 == 0) { // The method used is probably windows specific and should be replaced

				// Calculate FPS
				FPS = 1.0 / fpsTimer_elapsedTime;

				// Update Window Title
				SetWindowText(g_hWnd, (std::string(g_TutorialName) + std::string(" - ") + std::to_string((int)FPS) + std::string(" FPS ")).c_str());
			}

			// RandomizeBackgroundColor();
			RenderFrame();
			
			fpsTimer_t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // timestamp in nanoseconds
		}
	}

	// Shutdown Direct3D before exiting the program
	ShutdownD3D();

	return (int) msg.wParam;
}

//////////////////////////////////////////////////////////////////////////////////////
// Register class and create window
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Give your app window your own name
	char Name[100] = "Konstantin Schaper\0";
	// Register class
	WNDCLASSEX wcex={0};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wcex.hbrBackground = (HBRUSH )( COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;
	if(!RegisterClassEx(&wcex)) return E_FAIL;
	// Create window
	g_hInst = hInstance;
	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow( Name, g_TutorialName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if(!g_hWnd)
		return E_FAIL;
	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Called every time the application receives a message
//////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message)
	{

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(g_hWnd);
		else if (wParam == VK_LEFT)
		{
			g_pMainCamera->Rotate(-1.0F);
		}
		else if (wParam == VK_RIGHT)
		{
			g_pMainCamera->Rotate(1.0F);
		}
		else if (wParam == VK_UP)
		{
			g_pMainCamera->Forward(1.0F);
		}
		else if (wParam == VK_DOWN)
		{
			g_pMainCamera->Forward(-1.0F);
		}
		return 0;

	case WM_SIZE:
        if (g_pSwapChain)
        {
            g_pImmediateContext->OMSetRenderTargets(0, 0, 0);

            // Release all outstanding references to the swap chain's backbuffers.
            g_pBackBufferRTView->Release();

            HRESULT hr;

            // Preserve the existing buffer count and format.
            // Automatically choose the width and height to match the client rect for HWNDs.
            hr = g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
                                            
            // Perform error handling here!

			// Configure Z-Buffer texture
			D3D11_TEXTURE2D_DESC tex2dDesc;
			ZeroVariable(tex2dDesc);

			tex2dDesc.Width = LOWORD(lParam);
			tex2dDesc.Height = HIWORD(lParam);
			tex2dDesc.ArraySize = 1;
			tex2dDesc.MipLevels = 1;
			tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			tex2dDesc.SampleDesc.Count = 1;
			tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

			ID3D11Texture2D *pZBufferTexture;
			hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

			if (FAILED(hr)) return hr;

			// Create the Z buffer
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			ZeroMemory(&dsvDesc, sizeof(dsvDesc));

			dsvDesc.Format = tex2dDesc.Format;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			g_pD3DDevice->CreateDepthStencilView(pZBufferTexture,
				&dsvDesc, &g_pZBuffer);
			pZBufferTexture->Release();

            // Get back buffer and create a render-target-view.
            ID3D11Texture2D* pBuffer;
            hr = g_pSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),
                                         (void**) &pBuffer );
            // Perform error handling here!

            hr = g_pD3DDevice->CreateRenderTargetView(pBuffer, NULL,
                                                     &g_pBackBufferRTView);
            // Perform error handling here!
            pBuffer->Release();

            g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, NULL);

            // Set up the viewport.
            D3D11_VIEWPORT vp;
			vp.Width = LOWORD(lParam);
            vp.Height = HIWORD(lParam);
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            g_pImmediateContext->RSSetViewports( 1, &vp );
        }
        return 1;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Create D3D device and swap chain
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseD3D()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0
								//functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&g_pD3DDevice, &g_featureLevel, &g_pImmediateContext);
		if(SUCCEEDED(hr))
			break;
	}
	if(FAILED(hr))
		return hr;

	// Configure Z-Buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroVariable(tex2dDesc);

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// Create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	g_pD3DDevice->CreateDepthStencilView(pZBufferTexture,
		&dsvDesc, &g_pZBuffer);
	pZBufferTexture->Release();

	// Get pointer to back buffer texture
	ID3D11Texture2D *pBackBufferTexture;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);
	if(FAILED(hr)) return hr;
	// Use the back buffer texture pointer to create the render target view
	hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&g_pBackBufferRTView);
	pBackBufferTexture->Release();
	if(FAILED(hr)) return hr;
	// Set the render target view
	g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);
	// Set the viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	g_pImmediateContext->RSSetViewports(1, &viewport);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Initialize Buffers
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseGraphics(void)
{
	HRESULT hr = S_OK;

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU + GPU usage
	bufferDesc.ByteWidth = sizeof(g_pVertices); // Buffersize
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use created Buffer as Vertex Buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU access
	hr = g_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &g_pVertexBuffer); // Finally attempts to create the buffer

	if (FAILED(hr)) { // Returns error on failure
		return hr;
	}

	UpdateVertices();

	// Load and compile shaders
	ID3DBlob *VS, *PS, *error;

	// Vertexshader
	hr = D3DCompileFromFile(g_pShaderFileName, 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &error);

	if (error != 0) { // Check for shader compilation error
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) { // Dont fail if erro is just a warning
			return hr;
		}
	}

	// Pixelshader
	hr = D3DCompileFromFile(g_pShaderFileName, 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &error);

	if (error != 0) { // Check for shader compilation error
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) { // Dont fail if erro is just a warning
			return hr;
		}
	}

	// Create shader objects

	// Vertex Shader
	hr = g_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVertexShader);

	if (FAILED(hr)) return hr;

	// Pixel Shader
	hr = g_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPixelShader);

	if (FAILED(hr)) return hr;

	// Set the shader objects as active
	g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);

	// Create nad set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = g_pD3DDevice->CreateInputLayout(iedesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);

	if (FAILED(hr)) return hr;

	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	// Create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = sizeof(CONSTANT_BUFFER0); // Must be multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as constant buffer

	hr = g_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &g_pConstantBuffer0);

	if (FAILED(hr)) return hr;

	// Initialize Camera

	g_pMainCamera = new Camera(0.0F, 0.0F, -0.5F, 0.0F);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Clean up D3D objects
//////////////////////////////////////////////////////////////////////////////////////
void ShutdownD3D()
{
	delete g_pMainCamera;
	SafeRelease(g_pZBuffer);
	SafeRelease(g_pConstantBuffer0);
	if(g_pVertexBuffer) g_pVertexBuffer->Release(); // 03-01
	if (g_pInputLayout) g_pInputLayout->Release(); // 03-01
	if (g_pVertexShader) g_pVertexShader->Release(); // 03-01
	if (g_pPixelShader) g_pPixelShader->Release(); // 03-01
	if(g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if(g_pSwapChain) g_pSwapChain->Release();
	if(g_pImmediateContext) g_pImmediateContext->Release();
	if(g_pD3DDevice) g_pD3DDevice->Release();
}

//////////////////////////////////////////////////////////////////////////////////////
// Render frame
//////////////////////////////////////////////////////////////////////////////////////
void RenderFrame(void)
{

	// Get Window size
	RECT tarWnd;
	GetClientRect(g_hWnd, &tarWnd);
	float width = abs(tarWnd.right - tarWnd.left);
	float height = abs(tarWnd.bottom - tarWnd.top);
	float aspectRatio = height == 0.0F ? width : width / height;

	// Define translation matrices
	scale = DirectX::XMMatrixScaling(1.0F, 1.0F, 1.0F);
	rotation = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(x)) * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(y));
	translation = DirectX::XMMatrixTranslation(1.0F, 0.0F, 5.0F);

	world_transform = scale * rotation * translation;
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0), aspectRatio, 1.0, 100.0);

	DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0, 0.0, -5.0, 0.0);
	DirectX::XMVECTOR lookat = DirectX::XMVectorSet(0.0, 0.0, -4.0, 0.0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0, 1.0, 0.0, 0.0);
	view = g_pMainCamera->GetViewMatrix();

	WorldViewProjection = world_transform * view * projection;
	
	// Create constant buffer values
	CONSTANT_BUFFER0 cb0_values;
	cb0_values.RedAmount = 1.0F;
	cb0_values.scale = 1.0F;
	cb0_values.WorldViewProjection = WorldViewProjection;

	// Apply constant buffer
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer0, 0, 0, &cb0_values, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

	// Clear the back buffer - choose a colour you like
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, g_clear_colour);

	// Clear ZBuffer
	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);
	
	// Set vertex buffer // 03-01
	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Select which primitive type to use // 03-01
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

	// Draw the vertex buffer to the back buffer // 03-01
	g_pImmediateContext->Draw(sizeof(g_pVertices) / sizeof(g_pVertices[0]), 0);

	// Display what has just been rendered
	g_pSwapChain->Present(0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////
// Randomize background color values
//
// See http://stackoverflow.com/questions/18449173/timestamp-in-milliseconds-in-windows
// and https://msdn.microsoft.com/en-us/library/windows/desktop/ms724408(v=vs.85).aspx 
// for information about the timestamp used
//////////////////////////////////////////////////////////////////////////////////////
void RandomizeBackgroundColor() {

	// Get Current timestamp
	long int current_timestamp = GetTickCount();

	// Every 0.5s, randomize red value
	if (current_timestamp - red_timestamp >= 500) {
		red_timestamp = current_timestamp;
		RandomizeRed();
	}

	// Every second, randomize blue value
	if (current_timestamp - blue_timestamp >= 1000) {
		blue_timestamp = current_timestamp;
		RandomizeBlue();
	}

	// Every 1.5 seconds, randomize green value
	if (current_timestamp - green_timestamp >= 1500) {
		green_timestamp = current_timestamp;
		RandomizeGreen();
	}
}

// See http://www.cplusplus.com/reference/cstdlib/rand/ for information about generating random numbers

void RandomizeRed() {
	g_clear_colour[0] = (float) (rand() % 101) / 100.0F;
}

void RandomizeGreen() {
	g_clear_colour[1] = (float) (rand() % 101) / 100.0F;
}

void RandomizeBlue() {
	g_clear_colour[2] = (float) (rand() % 101) / 100.0F;
}

void UpdateVertices() {
	// Copy vertices to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// Copy the data
	memcpy(ms.pData, g_pVertices, sizeof(g_pVertices));

	// Unlock the buffer
	g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);
}