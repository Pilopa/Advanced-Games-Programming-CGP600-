#pragma once

#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public Renderer {

	public:
		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void draw(); // Overrides Renderer:draw

	private:
		Mesh* mesh;
		Material* material;

};