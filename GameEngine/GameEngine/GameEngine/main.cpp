#pragma once

#include <windows.h>
#include "ApplicationManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Exceptions.h"
#include "Debug.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Build Test Scene
	Scene scene = Scene();

	GameObject* object1 = new GameObject();
	GameObject* object2 = new GameObject();
	object2->setParent(object1);

	scene.addRootObject(object1);

	// Assign scene to game if possible
	if (GameManager::instance())
		GameManager::instance()->setScene(&scene);

	try {
		// Initialize Application
		ApplicationManager::initialize(new ApplicationManager("TinyTanks", CS_HREDRAW | CS_VREDRAW, { 0, 0, 640, 480 }, hInstance, nCmdShow));

		// Start Application Loop
		return ApplicationManager::instance()->executeMessageLoop();
	}
	catch (std::exception& exception) {
		LogError(exception.what());
		return 0;
	}

}