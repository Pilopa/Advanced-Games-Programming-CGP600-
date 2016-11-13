#pragma once

#include "GameObject.h"

GameObject::GameObject()
{
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

	// Add this object to the new parent's child list
	if (this->parent) this->parent->children.insert(this);
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

Transform * GameObject::getTransform()
{
	return &transform;
}