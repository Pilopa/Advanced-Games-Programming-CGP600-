#pragma once

#include "Renderer.h"
#include "ClassDef.h"

class MeshRenderer : public Renderer {

	public:
		void update(); // Overrides Component::update
		void draw(); // Overrides Renderer:draw

		MeshRenderer(Mesh* mesh, Material* material);

		Material* getMaterial();

	private:
		Mesh* mesh;
		Material* material;

};