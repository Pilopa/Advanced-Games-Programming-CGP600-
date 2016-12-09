#pragma once

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
	for (std::set<GameObject*>::const_iterator iterator = rootGameObjects.begin(), end = rootGameObjects.end(); iterator != end; ++iterator) {
		((GameObject*) *iterator)->update();
	}
}

void Scene::addRootObject(GameObject * object)
{
	rootGameObjects.insert(object);
	object->setScene(this);
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

