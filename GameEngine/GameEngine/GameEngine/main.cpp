#pragma once

#include <vector>
#include <windows.h>
#include "ApplicationManager.h"
#include "GameManager.h"
#include "Scene.h"
#include "Exceptions.h"
#include "Debug.h"
#include "SceneLoader.h"
#include "DeferredShaderClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int result;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	try {

		// Initialize Application
		ApplicationManager::initialize(new ApplicationManager("TinyTanks", CS_HREDRAW | CS_VREDRAW, { 0, 0, 640, 480 }, hInstance, nCmdShow));
		
		// Load Shaders : MUST BE DONE _HERE_ !!!
		DeferredShaderClass::INSTANCE = new DeferredShaderClass();

		// Initialize Scenes
		SceneLoader::setupScenes(DeferredShaderClass::INSTANCE);

		// Load start scene
		GameManager::instance()->setScene(SceneLoader::loadScene(0));

		// Start Application Loop
		result = ApplicationManager::instance()->executeMessageLoop();

		// Shutdown the application properly
		ApplicationManager::instance()->shutdown();

	}
	catch (std::exception& exception) {
		LogError((std::string("ErrorMessage: ") + std::string(exception.what())).c_str());
		return 0;
	}

	return result;
}