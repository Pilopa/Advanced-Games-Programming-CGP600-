#pragma once

#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public Renderer {

	public:
		void update(); // Overrides Component::update
		void draw(); // Overrides Renderer:draw

		MeshRenderer(Mesh* mesh, Material* material);

	private:
		Mesh* mesh;
		Material* material;

};