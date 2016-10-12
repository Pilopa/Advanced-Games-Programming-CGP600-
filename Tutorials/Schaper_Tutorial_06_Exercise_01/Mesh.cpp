#include "Mesh.h"

class Mesh::impl {

	private:
		POS_COL_VERTEX vertices[];

};

Mesh::Mesh() : pimpl{ new impl() } {}

Mesh::~Mesh(){} // Default Constructor

Mesh::Mesh(POS_COL_VERTEX * vertices)
{
	
}


POS_COL_VERTEX * Mesh::GetVertices()
{
	return nullptr;
}
