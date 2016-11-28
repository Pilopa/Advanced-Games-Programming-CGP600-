#pragma once

#include "Component.h"

class FreeFlightCameraScript : public Component {

	void update(); // Overrides Component::update
	void awake(); // Overrides Component::awake
	void shutdown(); // Overrides Component::shutdown

};