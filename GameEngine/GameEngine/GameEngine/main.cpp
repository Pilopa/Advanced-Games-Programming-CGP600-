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

// Defines a triangle of vertices, with each vertex having a distinct color
const std::vector<Vertex> testCubeVertices =
{
	// back face
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(1.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 1.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 1.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(1.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(1.0F, 1.0F), DirectX::XMFLOAT3(0.0F, 0.0F, -1.0F) },

	// front face
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },

	// left face
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // green
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // yellow
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // green
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // blue
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // yellow

																						  // right face
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },

	// bottom face
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // blue
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // green
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // red
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // purple
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }, // green

																						   // top face
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) }

};

const std::vector<UINT> testCubeIndices = {
	0,1,2,3,4,5,6,7,8,9,10,
	11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,
	29,30,31,32,33,34,35
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	try {
		// Initialize Application
		ApplicationManager::initialize(new ApplicationManager("TinyTanks", CS_HREDRAW | CS_VREDRAW, { 0, 0, 640, 480 }, hInstance, nCmdShow));

		// Load Shaders
		DeferredShaderClass deferredShaderClass = DeferredShaderClass();

		// Build Test Scene
		Scene scene = Scene();

		GameObject mainCameraObject = GameObject();
		mainCameraObject.getTransform()->localPosition->x = 0.0F;
		mainCameraObject.getTransform()->localPosition->y = 0.0F;
		mainCameraObject.getTransform()->localPosition->z = -5.0F;

		float sunColor[4] = { 1.0F, 0.0F, 0.0F, 1.0F };
		DirectionalLight directionalLight = DirectionalLight(sunColor, 1.0F);
		mainCameraObject.addComponent(&directionalLight);

		Camera mainCamera = Camera(PERSPECTIVE, 70.0F, 1.0F, 100.0F, 0.0F);
		mainCameraObject.addComponent(&mainCamera);
		
		FreeFlightCameraScript freeFlightScript = FreeFlightCameraScript();
		mainCameraObject.addComponent(&freeFlightScript);
		
		GameObject cube = GameObject();
		cube.getTransform()->localPosition->x = 0.0F;
		cube.getTransform()->localPosition->y = 0.0F;
		cube.getTransform()->localPosition->z = 1.0F;
		Mesh* cubeMesh = FileManager::loadObjMesh(L"FinalBaseMesh.obj"); //Mesh(testCubeVertices, testCubeIndices);
		Texture cubeTexture = Texture(L"cubeTexture.bmp");
		Material cubeMaterial = Material(&deferredShaderClass, &cubeTexture);
		MeshRenderer cubeRenderer = MeshRenderer(cubeMesh, &cubeMaterial);
		cube.addComponent(&cubeRenderer);

		GameObject cube2 = GameObject();
		cube2.getTransform()->localPosition->x = -5.0F;
		cube2.getTransform()->localPosition->y = 0.0F;
		cube2.getTransform()->localPosition->z = -1.0F;
		cube2.getTransform()->localRotation->yaw = 45.0F;
		MeshRenderer cube2Renderer = MeshRenderer(cubeMesh, &cubeMaterial);
		cube2.addComponent(&cube2Renderer);

		scene.setActiveCamera(&mainCamera);
		scene.addRootObject(&mainCameraObject);
		scene.addRootObject(&cube);
		scene.addRootObject(&cube2);

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
		return ApplicationManager::instance()->executeMessageLoop();
	}
	catch (std::exception& exception) {
		LogError(exception.what());
		return 0;
	}

}