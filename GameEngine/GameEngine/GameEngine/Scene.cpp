#pragma once

#include <algorithm>
#include "Scene.h"
#include "GameObject.h"
#include "Debug.h"
#include "FileManager.h"
#include "Texture3D.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "SkyboxShaderClass.h"

void Scene::awake()
{
	for (std::set<GameObject*>::const_iterator iterator = rootGameObjects.begin(), end = rootGameObjects.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->awake();
	}
}

void Scene::shutdown()
{
	LogInfo(std::to_string(rootGameObjects.size()).c_str());
	for (std::set<GameObject*>::const_iterator iterator = rootGameObjects.begin(), end = rootGameObjects.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->shutdown();
	}
}

void Scene::update()
{
	// Update status of objects in queue
	for (std::set<GameObject*>::const_iterator iterator = updateQueue.begin(), end = updateQueue.end(); iterator != end; ++iterator) {
		// Delete if present
		if (rootGameObjects.count(*iterator)) {
			rootGameObjects.erase(*iterator);
			(*iterator)->shutdown();
			(*iterator)->setScene(nullptr);
		}
		// Add if absent
		else {
			rootGameObjects.insert(*iterator);
		}
	}
	updateQueue.clear();
	
	// Iteratively update objects
	for (std::set<GameObject*>::const_iterator iterator = rootGameObjects.begin(), end = rootGameObjects.end(); iterator != end; ++iterator) {
		(*iterator)->update();
	}
}

void Scene::addRootObject(GameObject * object)
{
	rootGameObjects.insert(object);
	object->setScene(this);
}

void Scene::removeRootObject(GameObject * object)
{
	if (rootGameObjects.count(object))
		updateQueue.insert(object);
}

void Scene::setActiveCamera(Camera * camera)
{
	this->activeCamera = camera;
}

Camera * Scene::getActiveCamera()
{
	return activeCamera;
}

void Scene::setAmbientLight(AmbientLight * ambientLight)
{
	this->ambientLight = ambientLight;
}

AmbientLight * Scene::getAmbientLight()
{
	return ambientLight;
}

Renderer * Scene::getSkybox()
{
	return skybox;
}

Scene::Scene()
{
	GameObject* skyboxGameObject = new GameObject();
	skyboxGameObject->setScene(this);
	Mesh* skyboxMesh = FileManager::loadObjMesh(L"sphere.obj");
	Texture3D* skyboxTexture = new Texture3D(L"Skybox.dds");
	Material* skyboxMaterial = new Material(new SkyboxShaderClass(), skyboxTexture, 0.0f);
	MeshRenderer* skyboxRenderer = new MeshRenderer(skyboxMesh, skyboxMaterial);
	skyboxGameObject->addComponent(skyboxRenderer);
	this->skybox = skyboxRenderer;
}

void Scene::createGameObject(GameObject * object)
{
	updateQueue.insert(object);
	object->setScene(this);
}

