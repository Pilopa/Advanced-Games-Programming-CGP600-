#pragma once

#include "Scene.h"

void Scene::awake()
{
	for (std::set<GameObject*>::const_iterator iterator = rootGameObjects.begin(), end = rootGameObjects.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->awake();
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
}

void Scene::setActiveCamera(Camera * camera)
{
	this->activeCamera = camera;
}
