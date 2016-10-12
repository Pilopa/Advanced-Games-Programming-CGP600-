#pragma once
#include "Vertex.h"
#include <memory>

class Mesh {
	public:
		Mesh();
		~Mesh();
		Mesh(POS_COL_VERTEX *vertices);
		POS_COL_VERTEX* GetVertices();
	private:
		class impl;
		std::unique_ptr<impl> pimpl;
};