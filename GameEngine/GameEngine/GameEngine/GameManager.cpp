#pragma once

#include <string>
#include <chrono>
#include "GameManager.h"
#include "GraphicsManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "Debug.h"
#include "Scene.h"

using namespace std::chrono;

GameManager::GameManager()
{

}

GameManager* GameManager::s_instance = nullptr;

GameManager * GameManager::instance()
{
	if (!s_instance)
		s_instance = new GameManager();
	return s_instance;
}

void GameManager::executeGameLoopTick()
{
	// Reset Input
	InputManager::instance()->reset();

	// Calculate deltaTime
	long long tmpTimer = timer;
	if (timer == 0) {
		timer = duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
	else {
		timer = duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		deltaTime = (timer - tmpTimer);
	}

	// Call Update if there is an active scene
	if (scene) scene->update();

	// Do collision checks
	CollisionManager::instance()->performCollisionCheck();

	// Render next Frame
	if (GraphicsManager::instance()) GraphicsManager::instance()->renderFrame();
}

double GameManager::getDeltaTime()
{
	return deltaTime / 1000.0;
}

void GameManager::setScene(Scene * scene)
{
	// Dont bother if the given scene is already the active one
	if (this->scene != scene) {

		// Perform shutdown call on old scene
		if (this->scene)
			this->scene->shutdown();

		this->scene = scene;

		// Perform awake call on new scene
		if (this->scene)
			this->scene->awake();
	}
}

Scene * GameManager::getScene()
{
	return scene;
}

void GameManager::shutdown()
{
	if (GraphicsManager::instance()) {
		if (getScene()) {
			getScene()->shutdown();
		}
		GraphicsManager::instance()->shutdown();
		s_instance = nullptr;
		delete this;
	}

}
