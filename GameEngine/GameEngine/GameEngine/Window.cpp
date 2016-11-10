#pragma once

#include "Window.h"
#include "Exceptions.h"
#include "Debug.h"

ATOM Window::RegisterClass(char * name, UINT style, HINSTANCE hInstance, WNDPROC wndProc)
{
	// Register Class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = style;
	wcex.lpfnWndProc = wndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = name;

	// Attempt to register window class
	return RegisterClassEx(&wcex);
}

Window::Window(ATOM windowClass, HINSTANCE hInstance, char* title, RECT dimensions, int nCmdShow)
{
	if (FAILED(init(windowClass, hInstance, title, dimensions, nCmdShow)))
		throw WindowCreationException();
}

void Window::setTitle(LPCSTR title)
{
	SetWindowText(windowInstance, title);
}

void Window::getBounds(RECT * out)
{
	GetWindowRect(windowInstance, out);
}

HWND Window::getWinAPIWindowInstance()
{
	return windowInstance;
}

float Window::getAspectRatio()
{
	RECT bounds;
	GetClientRect(windowInstance, &bounds);
	long width = abs(bounds.right - bounds.left);
	long height = abs(bounds.bottom - bounds.top);
	return height == 0L ? (width == 0L ? 1 : width) : (float) width / (float) height;
}

HRESULT Window::init(ATOM windowClass, HINSTANCE hInstance, char * title, RECT dimensions, int nCmdShow)
{
	// Initialize Values
	this->handleInstance = hInstance;
	this->windowClass = windowClass;

	// Create window
	AdjustWindowRect(&dimensions, WS_OVERLAPPEDWINDOW, FALSE);
	windowInstance = CreateWindow(MAKEINTATOM(windowClass), title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, dimensions.right - dimensions.left,
		dimensions.bottom - dimensions.top, NULL, NULL, hInstance, NULL);

	// Return failure if the window could not be created
	if (!windowInstance) return E_FAIL;

	// Show the window if desired
	ShowWindow(windowInstance, nCmdShow);

	// Return ok value if everything went fine
	return S_OK;
}