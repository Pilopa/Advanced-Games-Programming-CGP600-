#pragma once

class Component {

	public:
		virtual void update() = 0;
		virtual void awake() = 0;
};