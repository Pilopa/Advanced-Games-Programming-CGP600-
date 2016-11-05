#pragma once

#include "ApplicationManager.h"
#include "Exceptions.h"

/* Static Declarations */

ApplicationManager* ApplicationManager::s_instance = nullptr;

LRESULT ApplicationManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}

/* Non-Static Declarations */

ApplicationManager::ApplicationManager(char * title, UINT windowStyle, RECT initialWindowDimensions, HINSTANCE hInstance, int nCmdShow)
{
	if (FAILED(init(title, windowStyle, initialWindowDimensions, hInstance, nCmdShow)))
		throw ApplicationManagerCreationException();
}


void ApplicationManager::initialize(ApplicationManager * app)
{
	if (!s_instance)
		s_instance = app;
	else
		throw AlreadyInitializedException("ApplicationManager");
}

HRESULT ApplicationManager::init(char * title, UINT windowStyle, RECT initialWindowDimensions, HINSTANCE hInstance, int nCmdShow)
{
	try {
		ATOM windowClass = Window::RegisterClass("MainWindow", windowStyle, hInstance, ApplicationManager::WndProc);
		window = new Window(windowClass, hInstance, title, initialWindowDimensions, nCmdShow);
		return S_OK;
	}
	catch (const WindowCreationException& exception) {
		UNREFERENCED_PARAMETER(exception);
		throw ApplicationManagerCreationException();
	}
}

int ApplicationManager::executeMessageLoop()
{
	// Main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Execute GameManager update if present
			if (GameManager::instance())
				GameManager::instance()->executeGameLoopTick();
		}
	}

	return (int) msg.wParam;
}

Window * ApplicationManager::getWindow()
{
	return window;
}
