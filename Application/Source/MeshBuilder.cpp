#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;


	v.pos.Set(-lengthX, 0, 0); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0, 0); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0, -lengthY, 0); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY, 0); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, -lengthZ); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float sizeX, float sizeZ, float tileSize) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	v.normal.Set(0, 1, 0);
	v.pos.Set(-1 * sizeX, 0, 1 * sizeZ); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(1 * sizeX, 0, 1 * sizeZ);  v.texCoord.Set(1 * tileSize, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-1 * sizeX, 0, -1 * sizeZ);  v.texCoord.Set(0, 1 * tileSize); vertex_buffer_data.push_back(v);
	v.pos.Set(1 * sizeX, 0, -1 * sizeZ);  v.texCoord.Set(1 * tileSize, 1 * tileSize); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateUIQuad(const std::string &meshName, Color color, float sizeX, float sizeZ, float tileSize) { //for hp bar

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	v.normal.Set(0, 1, 0);
	v.pos.Set(0 * sizeX, -1, 0 * sizeZ); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(2 * sizeX, -1, 0 * sizeZ);  v.texCoord.Set(1 * tileSize, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0 * sizeX, 1, 0 * sizeZ);  v.texCoord.Set(0, 1 * tileSize); vertex_buffer_data.push_back(v);
	v.pos.Set(2 * sizeX, 1, 0 * sizeZ);  v.texCoord.Set(1 * tileSize, 1 * tileSize); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateDiamondQuad(const std::string &meshName, Color color, float sizeX, float sizeY) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	// Normals facing forward because this mesh is standing straight up
	v.normal.Set(0, 0, 1);
	v.pos.Set(0, -1 * sizeY, 0); vertex_buffer_data.push_back(v); // Bot Mid point
	v.pos.Set(1 * sizeX, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1 * sizeY, 0); vertex_buffer_data.push_back(v); // Top Mid point
	v.pos.Set(-1 * sizeX, 0, 0); vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float sizeX, float sizeY, float sizeZ) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	// Front
	v.normal.Set(0, 0, 1);
	v.pos.Set(-1.0f * sizeX, -1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);

	// Right
	v.normal.Set(1, 0, 0);
	v.pos.Set(1.0f * sizeX, -1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);

	// Top
	v.normal.Set(0, 1, 0);
	v.pos.Set(-1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f * sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);

	// Left
	v.normal.Set(-1, 0, 0);
	v.pos.Set(-1.0f * sizeX, -1.0f * sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);

	// Back
	v.normal.Set(0, 0, -1);
	v.pos.Set(1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, 1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, 1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);

	// Bottom
	v.normal.Set(0, -1, 0);
	v.pos.Set(1.0f * sizeX, -1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f* sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v);
	v.pos.Set(1.0f * sizeX, -1.0f* sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v);


	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}


Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlices, float radius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.f / numSlices;

	for (unsigned slice = 0; slice <= numSlices; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			0,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(0, 1, 0);

		vertex_buffer_data.push_back(v);

	}

	v.pos.Set(0, 0, 0);
	v.color = color;
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i <= numSlices; i++) {
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(vertex_buffer_data.size() - 1);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlices, float height, float radius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.f / numSlices;

	for (unsigned slice = 0; slice <= numSlices; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			0,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(
			cos(Math::DegreeToRadian(theta)),
			0,
			sin(Math::DegreeToRadian(theta))
			).Normalize();
		vertex_buffer_data.push_back(v);

	}

	// Bottom-center of cylinder
	v.pos.Set(0, 0, 0);
	v.color = color;
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	// Top point of cylinder
	v.pos.Set(0, height, 0);
	v.color = color;
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i <= numSlices; i++) { // Top
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(vertex_buffer_data.size() - 1);
	}

	index_buffer_data.push_back(0); // Go back to slice

	for (unsigned i = 0; i <= numSlices; i++) { // Bottom
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(vertex_buffer_data.size() - 2);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numSlices, float height, float radius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.f / numSlices;

	// Bottom
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

	}

	v.pos.Set(0, -height * 0.5f, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	// Curved
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(
			cos(Math::DegreeToRadian(theta)),
			0,
			sin(Math::DegreeToRadian(theta))
			).Normalize();
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(
			cos(Math::DegreeToRadian(theta)),
			0,
			sin(Math::DegreeToRadian(theta))
			).Normalize();
		vertex_buffer_data.push_back(v);

	}

	// Top
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

	}

	v.pos.Set(0, height * 0.5f, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < numSlices + 1; ++i) {
		index_buffer_data.push_back(numSlices + 1);
		index_buffer_data.push_back(i);
	}

	for (unsigned i = 0; i < numSlices + 1; ++i) {
		index_buffer_data.push_back(numSlices + 2 + i * 2 + 1);
		index_buffer_data.push_back(numSlices + 2 + i * 2);
	}

	for (unsigned i = 0; i < numSlices + 1; ++i) {
		index_buffer_data.push_back(numSlices + 2 + (numSlices + 1) * 2 + i);
		index_buffer_data.push_back(numSlices + 2 + (numSlices + 1) * 2 + numSlices + 1);
	}



	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateHollowCylinder(const std::string &meshName, Color color, unsigned numSlices, float height, float outerR, float innerR) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.f / numSlices;

	// Bottom
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			outerR * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			outerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			innerR * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			innerR * sin(Math::DegreeToRadian(theta))
			);
		v.color = color;
		vertex_buffer_data.push_back(v);

	}


	// Outer Curved
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			outerR * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			outerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(
			cos(Math::DegreeToRadian(theta)),
			0,
			sin(Math::DegreeToRadian(theta))
			).Normalize();
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			outerR * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			outerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		vertex_buffer_data.push_back(v);

	}

	// Inner Curved
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			innerR * cos(Math::DegreeToRadian(theta)),
			-height * 0.5f,
			innerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		v.normal.Set(
			cos(Math::DegreeToRadian(theta)),
			0,
			sin(Math::DegreeToRadian(theta))
			).Normalize();
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			innerR * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			innerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		vertex_buffer_data.push_back(v);

	}


	// Top
	for (unsigned slice = 0; slice < numSlices + 1; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(
			innerR * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			innerR * sin(Math::DegreeToRadian(theta))
			);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			outerR * cos(Math::DegreeToRadian(theta)),
			height * 0.5f,
			outerR * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		vertex_buffer_data.push_back(v);

	}

	for (unsigned i = 0; i < vertex_buffer_data.size(); i += 2) {
		index_buffer_data.push_back(i + 1);
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlices, float innerRadius, float OuterRadius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.f / numSlices;

	for (unsigned slice = 0; slice <= numSlices; ++slice) {

		float theta = slice * anglePerSlice;

		// Outer ring
		v.pos.Set(
			OuterRadius * cos(Math::DegreeToRadian(theta)),
			0,
			OuterRadius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		vertex_buffer_data.push_back(v);

		// Inner ring
		v.pos.Set(
			innerRadius * cos(Math::DegreeToRadian(theta)),
			0,
			innerRadius * sin(Math::DegreeToRadian(theta))
			);
		v.color = color;
		vertex_buffer_data.push_back(v);

	}

	for (unsigned i = 0; i < vertex_buffer_data.size(); ++i) {
		index_buffer_data.push_back(i);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}


Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numSlices, unsigned numStack, float radius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float anglePerSlice = 360.0f / numSlices;
	float anglePerStack = 180.0f / numStack;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) { //stack

		float phi = Math::DegreeToRadian(-90.0f + stack * anglePerStack);

		for (unsigned slice = 0; slice < numSlices + 1; ++slice) { //slice

			float theta = Math::DegreeToRadian(slice * anglePerSlice);

			v.pos.Set(
				radius * cos(phi) * cos(theta),
				radius * sin(phi),
				radius * cos(phi) * sin(theta)
				);
			v.color = color;
			v.normal.Set(
				cos(phi) * cos(theta),
				sin(phi),
				cos(phi) * sin(theta)
				).Normalize();
			vertex_buffer_data.push_back(v);

		}

	}

	for (unsigned stack = 0; stack < numStack; ++stack) { //stack

		for (unsigned slice = 0; slice < numSlices + 1; ++slice) { //slice

			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);

		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateHemisphere(const std::string &meshName, Color color, unsigned numSlices, unsigned numStack, float radius) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	for (unsigned phi = 0; phi < 90; phi += 10) { //stack

		for (unsigned theta = 0; theta <= (numSlices * 20); theta += 10) { //slice

			float theta1 = Math::DegreeToRadian((float)theta);
			float phi1 = Math::DegreeToRadian((float)phi);

			v.pos.Set(
				radius * x(phi1, theta1),
				radius * y(phi1, theta1),
				radius * z(phi1, theta1)
				);
			v.color = color;
			v.normal.Set(
				x(phi1, theta1),
				y(phi1, theta1),
				z(phi1, theta1)
				).Normalize();
			vertex_buffer_data.push_back(v);

			float phi2 = Math::DegreeToRadian((float)(phi + 10));

			v.pos.Set(
				radius * x(phi2, theta1),
				radius * y(phi2, theta1),
				radius * z(phi2, theta1)
				);
			v.color = color;
			v.normal.Set(
				x(phi2, theta1),
				y(phi2, theta1),
				z(phi2, theta1)
				).Normalize();
			vertex_buffer_data.push_back(v);

		}

	}

	float anglePerSlice = 360.f / numSlices;

	for (unsigned slice = 0; slice <= numSlices; ++slice) {

		float theta = slice * anglePerSlice;

		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(
			radius * cos(Math::DegreeToRadian(theta)),
			0,
			radius * sin(Math::DegreeToRadian(theta))
			);

		v.color = color;
		vertex_buffer_data.push_back(v);

	}


	for (unsigned i = 0; i <= vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR) {


	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float degreePerStack = Math::DegreeToRadian(360.f / numStack);
	float degreePerSlice = Math::DegreeToRadian(360.f / numSlice);
	float x1, z1; // y1 is always zero
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			z1 = outerR * cos(stack * degreePerStack);
			x1 = outerR * sin(stack * degreePerStack);
			z2 = (outerR + innerR * cos(slice * degreePerSlice)) * cos(stack * degreePerStack);
			y2 = innerR * sin(slice * degreePerSlice);
			x2 = (outerR + innerR * cos(slice * degreePerSlice)) * sin(stack * degreePerStack);

			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(x2 - x1, y2, z2 - z1).Normalize();
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateSquarePyramid(const std::string &meshName, Color color, float size, float height, float offsetX) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	// Front
	v.normal.Set(0, 0, 1);
	v.pos.Set(1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Right
	v.pos.Set(offsetX, height, 0); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(-1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Left

	// Left
	v.normal.Set(-1, 0, 0);
	v.pos.Set(-1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Left
	v.pos.Set(-1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Left
	v.pos.Set(offsetX, height, 0); vertex_buffer_data.push_back(v); // Tip of pyramid

	// Right
	v.normal.Set(1, 0, 0);
	v.pos.Set(offsetX, height, 0); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Right
	v.pos.Set(1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Right

	// Back
	v.normal.Set(0, 0, -1);
	v.pos.Set(1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Right
	v.pos.Set(offsetX, height, 0); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(-1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Left

	// Bottom
	v.normal.Set(0, -1, 0);
	v.pos.Set(-1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Left
	v.pos.Set(1 * size, 0, -1 * size); vertex_buffer_data.push_back(v); // B Right
	v.pos.Set(-1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Left
	v.pos.Set(1 * size, 0, 1 * size); vertex_buffer_data.push_back(v); // F Right

	for (unsigned i = 0; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateTrianglePyramid(const std::string &meshName, Color color, float size, float height, float offsetX, float offsetZ) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	// Front
	v.normal.Set(0, height / 2, 1).Normalize();
	v.pos.Set(1.0f * size, 0, 0.5f * size); v.texCoord.Set(.5, 0); vertex_buffer_data.push_back(v); // Right
	v.pos.Set(offsetX, height, offsetZ); v.texCoord.Set(0.25f, .5f); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(-1.0f * size, 0, 0.5f * size); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v); // Left

	// Left
	v.normal.Set(-1, height / 2, -1).Normalize();
	v.pos.Set(-1.0f * size, 0, 0.5f * size); v.texCoord.Set(0.5f, 0.5f); vertex_buffer_data.push_back(v); // Left
	v.pos.Set(offsetX, height, offsetZ); v.texCoord.Set(0.25f, 1); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(0, 0, -1.0f * size); v.texCoord.Set(0.0f, 0.5f); vertex_buffer_data.push_back(v); // Back

	// Right
	v.normal.Set(1, height / 2, -1).Normalize();
	v.pos.Set(0, 0, -1.0f * size);  v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v); // Back
	v.pos.Set(offsetX, height, offsetZ); v.texCoord.Set(0.75f, 0.5f); vertex_buffer_data.push_back(v); // Tip of pyramid
	v.pos.Set(1.0f * size, 0, 0.5f * size); v.texCoord.Set(0.5f, 0); vertex_buffer_data.push_back(v); // Right

	// Bottom
	v.normal.Set(0, -1, 0);
	v.pos.Set(-1.0f * size, 0, 0.5f * size); v.texCoord.Set(1, 0.5f);  vertex_buffer_data.push_back(v); // Left
	v.pos.Set(0, 0, -1 * size); v.texCoord.Set(0.75f, 1); vertex_buffer_data.push_back(v); // Back
	v.pos.Set(1.0f * size, 0, 0.5f * size); v.texCoord.Set(0.5f, 0.5f); vertex_buffer_data.push_back(v); // Right

	for (unsigned i = 0; i <= vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}

Mesh* MeshBuilder::GenerateTriangleCuboid(const std::string &meshName, Color color, float sizeX, float sizeY, float sizeZ) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.color = color;

	// Front
	v.normal.Set(0, 0, 1);
	v.pos.Set(1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f right 
	v.pos.Set(0, sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f up 
	v.pos.Set(-1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f left 

	// Left
	v.normal.Set(-0.5f, 0.5f, 0).Normalize();
	v.pos.Set(-1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f left 
	v.pos.Set(-1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b left 
	v.pos.Set(0, sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f up 
	v.pos.Set(0, sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b up 

	// Back
	v.normal.Set(0, 0, -1);
	v.pos.Set(0, sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b up 
	v.pos.Set(-1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b left 
	v.pos.Set(1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b right 

	// Right
	v.normal.Set(0.5f, 0.5f, 0).Normalize();
	v.pos.Set(1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b right 
	v.pos.Set(0, sizeY, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b up 
	v.pos.Set(1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f right 	
	v.pos.Set(0, sizeY, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f up 

	// Bottom
	v.normal.Set(0, -1, 0);
	v.pos.Set(1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f right 	
	v.pos.Set(-1.0f* sizeX, -1.0f, 1.0f * sizeZ); vertex_buffer_data.push_back(v); //f left 
	v.pos.Set(1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b right 
	v.pos.Set(-1.0f* sizeX, -1.0f, -1.0f * sizeZ); vertex_buffer_data.push_back(v); //b left 

	for (unsigned i = 0; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;

}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path) {

	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;

	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);

	if (!success)
		return NULL;

	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol) {

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	v.normal.Set(0, 0, 1);

	unsigned offset = 0;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	for (unsigned i = 0; i < numRow; ++i) {
		for (unsigned j = 0; j < numCol; ++j) {

			v.pos.Set(-1, 1, 0);  v.texCoord.Set(j * width, (numRow - i) * height); vertex_buffer_data.push_back(v); // top left
			v.pos.Set(-1, -1, 0); v.texCoord.Set(j * width, (numRow - i - 1) * height); vertex_buffer_data.push_back(v); // bot left
			v.pos.Set(1, -1, 0);  v.texCoord.Set((j + 1) * width, (numRow - i - 1) * height); vertex_buffer_data.push_back(v); // bot right            
			v.pos.Set(1, 1, 0);  v.texCoord.Set((j + 1) * width, (numRow - i) * height); vertex_buffer_data.push_back(v); // top right

			index_buffer_data.push_back(0 + offset);
			index_buffer_data.push_back(1 + offset);
			index_buffer_data.push_back(2 + offset);
			index_buffer_data.push_back(2 + offset);
			index_buffer_data.push_back(3 + offset);
			index_buffer_data.push_back(0 + offset);

			offset += 4;
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
