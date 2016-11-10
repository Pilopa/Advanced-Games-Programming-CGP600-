#pragma once

#include <windows.h>
#include <functional>
#include "Window.h"
#include "Exceptions.h"

class ApplicationManager {

	public:

		ApplicationManager(char* title, UINT windowStyle, RECT initialWindowDimensions, HINSTANCE hInstance, int nCmdShow);

		//
		int executeMessageLoop();
		Window* getWindow();

		static ApplicationManager* instance() {
			return s_instance;
		}

		static void initialize(ApplicationManager* app);


	private:

		static ApplicationManager* s_instance;
		Window* window;

		HRESULT init(char* title, UINT windowStyle, RECT initialWindowDimensions, HINSTANCE hInstance, int nCmdShow);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
		