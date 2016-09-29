#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////////////////

// Rename for each tutorial

char g_TutorialName[100] = "Tutorial 02 Exercise 01\0";

// Window Specific Variables

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

// Direct 3D Variables

D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pD3DDevice = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
IDXGISwapChain* g_pSwapChain = NULL;

// Other
float g_clear_colour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
long red_timestamp = 0;
long blue_timestamp = 0;
long green_timestamp = 0;
float x;
float y;
float tx;
float ty;
 
//////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
//////////////////////////////////////////////////////////////////////////////////////

HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
void RenderFrame(void);
void RandomizeBackgroundColor();
void RandomizeRed();
void RandomizeGreen();
void RandomizeBlue();
void ShutdownD3D();
float Clamp(float f, float lower, float upper);

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
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	// Attempt to initialize the Direct3D render engine,
	// exit the program with an error message on failure
	if(FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	// Initialize random seed
	srand (time(NULL));

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
			//RandomizeBackgroundColor();
			RenderFrame();
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

	case WM_LBUTTONDOWN:
		RandomizeRed();
		break;

	case WM_RBUTTONDOWN:
		RandomizeBlue();
		break;

	case WM_MBUTTONDOWN:
		RandomizeGreen();
		break;

	// See https://msdn.microsoft.com/en-us/library/windows/desktop/ms645617(v=vs.85).aspx
	case WM_MOUSEWHEEL:
		g_clear_colour[0] = Clamp(g_clear_colour[0] + (GET_WHEEL_DELTA_WPARAM(wParam) / 1200.0F), 0.0F, 1.0F);
		g_clear_colour[1] = Clamp(g_clear_colour[1] + (GET_WHEEL_DELTA_WPARAM(wParam) / 1200.0F), 0.0F, 1.0F);
		g_clear_colour[2] = Clamp(g_clear_colour[2] + (GET_WHEEL_DELTA_WPARAM(wParam) / 1200.0F), 0.0F, 1.0F);
		break;

	case WM_MOUSEMOVE: 
		tx = LOWORD(lParam);
		ty = HIWORD(lParam);

		g_clear_colour[0] = Clamp(g_clear_colour[0] + ((tx - x) / 1000.0F), 0.0F, 1.0F);
		g_clear_colour[1] = Clamp(g_clear_colour[1] + ((ty - y) / 1000.0F), 0.0F, 1.0F);

		x = tx;
		y = ty;

		break;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(g_hWnd);
		return 0;

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
	g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, NULL);
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
// Clean up D3D objects
//////////////////////////////////////////////////////////////////////////////////////
void ShutdownD3D()
{
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
	// Clear the back buffer - choose a colour you like
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, g_clear_colour);
	// RENDER HERE
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

float Clamp(float f, float lower, float upper) {
	return max(lower, min(f, upper));
}