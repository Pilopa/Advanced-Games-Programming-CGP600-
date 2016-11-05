#pragma once

#include <windows.h>

class Window {

	public:
		Window(ATOM windowClass, HINSTANCE hInstance, char* title, RECT dimensionse, int nCmdShow);
		void setTitle(LPCSTR title);
		void getBounds(RECT* out);
		HWND getWinAPIWindowInstance();

		static ATOM RegisterClass(char* name, UINT style, HINSTANCE hInstance, WNDPROC wndProc);

	private:

		HINSTANCE handleInstance;
		HWND windowInstance;
		ATOM windowClass;

		HRESULT init(ATOM windowClass, HINSTANCE hInstance, char* title, RECT dimensions, int nCmdShow);

};