#pragma once

#include "GameObject.h"
#include "CustomScript.h"
#include "Scene.h"

GameObject::GameObject()
{
	transform.setGameObject(this);
	components.insert(&transform);
}

void GameObject::update()
{
	// Update components
	for (std::set<Component*>::const_iterator iterator = components.begin(), end = components.end(); iterator != end; ++iterator) {
		((Component*)*iterator)->update();
	}

	// Update children
	for (std::set<GameObject*>::const_iterator iterator = children.begin(), end = children.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->update();
	}
}

void GameObject::awake()
{
	// Awake components
	for (std::set<Component*>::const_iterator iterator = components.begin(), end = components.end(); iterator != end; ++iterator) {
		((Component*)*iterator)->awake();
	}

	// Awake children
	for (std::set<GameObject*>::const_iterator iterator = children.begin(), end = children.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->awake();
	}
}

void GameObject::shutdown()
{
	// Shutdown components
	for (std::set<Component*>::const_iterator iterator = components.begin(), end = components.end(); iterator != end; ++iterator) {
		((Component*)*iterator)->shutdown();
	}

	// Shutdown children
	for (std::set<GameObject*>::const_iterator iterator = children.begin(), end = children.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->shutdown();
	}
}

void GameObject::destroy()
{
	// Remove from game loop
	getScene()->removeRootObject(this);
}

GameObject* GameObject::getParent() 
{
	return parent;
}

void GameObject::setParent(GameObject* parent)
{
	// Remove this object from previous parent's child list
	if (this->parent) this->parent->children.erase(this);

	// Assign parent value
	this->parent = parent;

	// If new parent is not nullptr
	if (this->parent) {

		// Set this scene to parent's scene
		this->scene = this->parent->getScene();

		// Add this object to the new parent's child list
		this->parent->children.insert(this);

	}
	else this->scene = nullptr;
}

void GameObject::addComponent(Component * component)
{
	components.insert(component);
	component->setGameObject(this);
}

Scene * GameObject::getScene()
{
	return scene;
}

void GameObject::setScene(Scene * scene)
{
	this->scene = scene;

	// Change scene of children
	for (std::set<GameObject*>::const_iterator iterator = children.begin(), end = children.end(); iterator != end; ++iterator) {
		((GameObject*)*iterator)->setScene(scene);
	}
}

void GameObject::onCollision(Collider * first, Collision * collision)
{
	// Call Custom Scripts to handle collision
	for (std::set<Component*>::const_iterator iterator = components.begin(), end = components.end(); iterator != end; ++iterator) {
		if (instanceof<CustomScript>(*iterator)) {
			((CustomScript*)*iterator)->onCollision(first, collision);
		}
	}
}

Transform * GameObject::getTransform()
{
	return &transform;
}