#pragma once

#include <d3d11.h>
#include "Component.h"
#include "SimpleMath.h"

class UIComponent : public Component {

	public:
		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown

		virtual void render() = 0;

		UIComponent(DirectX::SimpleMath::Vector2 position);

	protected:
		DirectX::SimpleMath::Vector2 position = { 0.0f, 0.0f };

};