#pragma once

#include <string>
#include <chrono>
#include "GameManager.h"
#include "Debug.h"

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
	// Calculate deltaTime
	long long tmpTimer = timer;
	timer = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	deltaTime = (timer - tmpTimer) / 1000000.0;
	LogInfo((std::string("deltaTime: ") + std::to_string(this->getDeltaTime()) + std::string("\n")).c_str());

	// Call Update if there is an active scene
	if (scene) scene->update();

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

		this->scene = scene;

		// Perform awake call
		if (this->scene)
			this->scene->awake();
	}
}