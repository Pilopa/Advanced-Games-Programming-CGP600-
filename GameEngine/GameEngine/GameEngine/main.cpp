#pragma once

#include <vector>
#include <windows.h>
#include "ApplicationManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Exceptions.h"
#include "Debug.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "FreeFlightCameraScript.h"
#include "DeferredShaderClass.h"
#include "DirectionalLight.h"
#include "Texture.h"
#include "FileManager.h"
#include "AmbientLight.h"
#include "GraphicsManager.h"
#include "PointLight.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int result;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	try {
		// Initialize Application
		ApplicationManager::initialize(new ApplicationManager("TinyTanks", CS_HREDRAW | CS_VREDRAW, { 0, 0, 640, 480 }, hInstance, nCmdShow));

		// Define Ambient Light
		float ambientColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
		AmbientLight ambientLight = AmbientLight(ambientColor, 0.1F);

		// Load Shaders
		DeferredShaderClass deferredShaderClass = DeferredShaderClass();

		// Build Test Scene
		Scene scene = Scene();

		GameObject mainCameraObject = GameObject();
		mainCameraObject.getTransform()->localPosition->x = 0.0F;
		mainCameraObject.getTransform()->localPosition->y = 0.0F;
		mainCameraObject.getTransform()->localPosition->z = -5.0F;

		// Initialize sun
		GameObject sunGameObject = GameObject();
		sunGameObject.getTransform()->localPosition->x = 0.0F;
		sunGameObject.getTransform()->localPosition->y = 2.0F;
		sunGameObject.getTransform()->localPosition->z = 4.0F;
		float sunColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
		DirectionalLight directionalLight = DirectionalLight(sunColor, 1.0F);
		sunGameObject.addComponent(&directionalLight);
		PointLight pointLight = PointLight(sunColor, 1.0F);
		sunGameObject.addComponent(&pointLight);

		Camera mainCamera = Camera(PERSPECTIVE, 70.0F, 1.0F, 100.0F, 0.0F);
		mainCameraObject.addComponent(&mainCamera);
		
		FreeFlightCameraScript freeFlightScript = FreeFlightCameraScript();
		mainCameraObject.addComponent(&freeFlightScript);
		
		GameObject cube = GameObject();
		cube.getTransform()->localPosition->x = 0.0F;
		cube.getTransform()->localPosition->y = 0.0F;
		cube.getTransform()->localPosition->z = 5.0F;
		Mesh* cubeMesh = FileManager::loadObjMesh(L"sphere.obj");
		Texture cubeTexture = Texture(L"cubeTexture.bmp");
		Material cubeMaterial = Material(&deferredShaderClass, &cubeTexture, 5.0f);
		MeshRenderer cubeRenderer = MeshRenderer(cubeMesh, &cubeMaterial);
		cube.addComponent(&cubeRenderer);
		//cube.setParent(&mainCameraObject);

		GameObject cube2 = GameObject();
		cube2.getTransform()->localPosition->x = -3.0F;
		cube2.getTransform()->localPosition->y = -1.0F;
		cube2.getTransform()->localPosition->z = 0.0F;
		MeshRenderer cube2Renderer = MeshRenderer(cubeMesh, &cubeMaterial);
		cube2.addComponent(&cube2Renderer);
		//cube2.setParent(&cube);

		scene.setAmbientLight(&ambientLight);
		scene.setActiveCamera(&mainCamera);
		scene.addRootObject(&mainCameraObject);
		scene.addRootObject(&cube);
		scene.addRootObject(&cube2);
		scene.addRootObject(&sunGameObject);
		
		// Assign scene to game
		GameManager::instance()->setScene(&scene);

		// Set up checks for memory leaks.
		// Game Coding Complete reference - Chapter 21, page 834
		//
		int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

		// set this flag to keep memory blocks around
		// tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;        // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

		// perform memory check for each alloc/dealloc
		tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;     // remember this is VERY VERY SLOW!

												   //_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
												   //   leak check just before program exit. This is important because
												   //   some classes may dynamically allocate memory in globally constructed
												   //   objects.
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

		_CrtSetDbgFlag(tmpDbgFlag);

		// Start Application Loop
		result = ApplicationManager::instance()->executeMessageLoop();

		// Shutdown the application properly
		ApplicationManager::instance()->shutdown();

	}
	catch (std::exception& exception) {
		LogError(exception.what());
		return 0;
	}

	return result;
}