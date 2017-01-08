#pragma once

#include "SceneLoader.h"
#include "Scene.h"
#include "AmbientLight.h"
#include "GameObject.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "FileManager.h"
#include "Texture.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "DeferredShaderClass.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "PlayerControllerScript.h"
#include "Mesh.h"
#include "Rigidbody.h"
#include "EnemyPrefab.h"
#include "EnemyBehaviorScript.h"

std::vector<Scene*> SceneLoader::scenes;

void SceneLoader::setupScenes(DeferredShaderClass* deferredShaderClass)
{
	// Define values
	float ambientColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
	float ambientLightIntensity = 0.1F;
	float sunColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
	float sunLightIntensity = 1.0f;
	float playerMatShininess = 5.0f;
	float planeMatShininess = 1.0f;

	// Scene 1

		// << Create scene object
		Scene* scene = new Scene();

		// Create Ambient Light
		
		AmbientLight* ambientLight = new AmbientLight(ambientColor, ambientLightIntensity);
		scene->setAmbientLight(ambientLight);

		// Create Directional Light (Sun)
		GameObject* sun = new GameObject();
		DirectionalLight* sunDirectionalLight = new DirectionalLight(sunColor, sunLightIntensity);
		sun->addComponent(sunDirectionalLight);
		scene->addRootObject(sun);

		// Create character
		GameObject* playerObject = new GameObject();
		playerObject->getTransform()->localPosition->y = 0.8f;
		Mesh* playerMesh = FileManager::loadObjMesh(L"sphere.obj");
		Texture* playerTexture = new Texture(L"cubeTexture.bmp");
		Material* playerMaterial = new Material(deferredShaderClass, playerTexture, playerMatShininess);
		MeshRenderer* playerRenderer = new MeshRenderer(playerMesh, playerMaterial);
		BoxCollider* playerCollider = new BoxCollider(CollisionManager::instance(), { 0.0F, 0.0F, 0.0F }, false, { 0.75f, 0.75f, 0.75f });
		Rigidbody* playerRigidbody = new Rigidbody();
		PlayerControllerScript* playerController = new PlayerControllerScript();

		playerObject->addComponent(playerRenderer);
		playerObject->addComponent(playerRigidbody);
		playerObject->addComponent(playerCollider);
		playerObject->addComponent(playerController);

		scene->addRootObject(playerObject);

		// Create main camera
		GameObject* mainCameraObject = new GameObject();
		mainCameraObject->getTransform()->localPosition->z = -4.0f;
		mainCameraObject->getTransform()->localPosition->y = 2.0f;
		mainCameraObject->getTransform()->localRotation->pitch = 12.5f; // degrees
		Camera* mainCamera = new Camera(PERSPECTIVE, 70.0F, 1.0F, 100.0F, 0.0F);
		mainCameraObject->addComponent(mainCamera);
		mainCameraObject->setParent(playerObject); // Attach to player
		scene->setActiveCamera(mainCamera); // Activate for rendering

		// Create enemy
		GameObject* enemy1 = EnemyPrefab::instance()->instantiate({0.0, 0.0, 4.0});
		scene->addRootObject(enemy1);
		enemy1->getComponent<EnemyBehaviorScript>()->setPlayerTransform(playerObject->getTransform());

		// Create plane object
		GameObject* groundObject = new GameObject();
		groundObject->getTransform()->localScale->x = 2.0f;
		groundObject->getTransform()->localScale->z = 2.0f;

		Mesh* planeMesh = FileManager::loadObjMesh(L"plane.obj");
		Texture* planeTexture = new Texture(L"concrete.jpg");
		Material* planeMaterial = new Material(deferredShaderClass, planeTexture, planeMatShininess);
		MeshRenderer* planeRenderer = new MeshRenderer(planeMesh, planeMaterial);
		BoxCollider* planeCollider = new BoxCollider(CollisionManager::instance(), { 0.0F, 0.0F, 0.0F }, false, { 10.0f, 0.1f, 10.0f });

		groundObject->addComponent(planeRenderer);
		groundObject->addComponent(planeCollider);

		scene->addRootObject(groundObject);

		// Add scene to list >>
		scenes.push_back(scene);
}

Scene * SceneLoader::loadScene(int index)
{
	return scenes[index];
}
