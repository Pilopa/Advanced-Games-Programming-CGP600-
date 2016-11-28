#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "Debug.h"

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

