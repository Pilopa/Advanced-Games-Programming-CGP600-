#pragma once

#include "ClassDef.h"

class GameManager {

	public:

		// Executes exactly one game tick
		void executeGameLoopTick();

		// Returns the elapsed time since the last frame in seconds
		double getDeltaTime();

		// Overrides the currently active scene with a new one,
		// displaying it immediatly
		void setScene(Scene* scene);

		Scene* getScene();

		static GameManager* instance();

	private:

		// Time elapsed since last frame in milliseconds
		double deltaTime = 0.0;
		long long timer = 0;
		Scene* scene = NULL;
		
		// Default Constructor
		GameManager();

		static GameManager* s_instance;
};