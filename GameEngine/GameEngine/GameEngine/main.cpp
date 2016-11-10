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

// Defines a triangle of vertices, with each vertex having a distinct color
const std::vector<VERTEX> testCubeVertices =
{
	// back face
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0F, 0.0F), DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F) },

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
		VertexShader* vs = VertexShader::loadFromFile(L"VertexShader.hlsl");
		PixelShader* ps = PixelShader::loadFromFile(L"PixelShader.hlsl");

		// Build Test Scene
		Scene scene = Scene();

		GameObject mainCameraObject = GameObject();
		mainCameraObject.getTransform()->localPosition->x = 0.0F;
		mainCameraObject.getTransform()->localPosition->y = 0.0F;
		mainCameraObject.getTransform()->localPosition->z = -5.0F;

		Camera mainCamera = Camera(PERSPECTIVE, 70.0F, 1.0F, 100.0F, 0.0F);
		mainCameraObject.addComponent(&mainCamera);
		
		FreeFlightCameraScript freeFlightScript = FreeFlightCameraScript();
		mainCameraObject.addComponent(&freeFlightScript);
		
		GameObject cube = GameObject();
		cube.getTransform()->localPosition->x = 0.0F;
		cube.getTransform()->localPosition->y = 0.0F;
		cube.getTransform()->localPosition->z = -1.0F;
		cube.getTransform()->localRotation->pitch = 45.0F;
		cube.getTransform()->localRotation->yaw = 45.0F;
		Mesh cubeMesh = Mesh(testCubeVertices, testCubeIndices);
		Material cubeMaterial = Material(vs, ps, nullptr);
		MeshRenderer cubeRenderer = MeshRenderer(&cubeMesh, &cubeMaterial);
		cube.addComponent(&cubeRenderer);

		scene.setActiveCamera(&mainCamera);
		scene.addRootObject(&mainCameraObject);
		scene.addRootObject(&cube);

		// Assign scene to game
		GameManager::instance()->setScene(&scene);

		// Start Application Loop
		return ApplicationManager::instance()->executeMessageLoop();
	}
	catch (std::exception& exception) {
		LogError(exception.what());
		return 0;
	}

}