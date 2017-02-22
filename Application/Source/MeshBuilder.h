#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
		\brief	Provides methods to generate mesh of different shapes
		*/
/******************************************************************************/
class MeshBuilder {
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float sizeX = 1, float sizeZ = 1, float tileSize = 1);
	static Mesh* GenerateUIQuad(const std::string &meshName, Color color, float sizeX = 1, float sizeZ = 1, float tileSize = 1);

	static Mesh* GenerateCube(const std::string &meshName, Color color, float sizeX = 1, float sizeY = 1, float sizeZ = 1);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices = 10, float radius = 1.0f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlices = 10, float height = 3.0f, float radius = 1.0f);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numSlices = 15, float height = 3.0f, float radius = 1.0f);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlices = 10, float innerRadius = 0.5f, float outerRadius = 1.0f);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numSlices = 16, unsigned numStack = 16, float radius = 1.0f);
	static Mesh* GenerateHemisphere(const std::string &meshName, Color color, unsigned numSlices = 32, unsigned numStack = 36, float radius = 1.0f);

	// Unique Meshes
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numStack = 18, unsigned numSlice = 18, float outerR = 2, float innerR = 1);
	static Mesh* GenerateDiamondQuad(const std::string &meshName, Color color, float sizeX = 1, float sizeY = 1);
	static Mesh* GenerateSquarePyramid(const std::string &meshName, Color color, float size = 1, float height = 1, float offsetX = 0);
	static Mesh* GenerateTrianglePyramid(const std::string &meshName, Color color, float size = 1, float height = 1, float offsetX = 0, float offsetZ = 0);
	static Mesh* GenerateTriangleCuboid(const std::string &meshName, Color color, float sizeX = 1, float sizeY = 1, float sizeZ = 1);
	static Mesh* GenerateHollowCylinder(const std::string &meshName, Color color, unsigned numSlices = 18, float height = 3.0f, float outerR = 2, float innerR = 1);

    // OBJ loader
    static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);

    // Text mesh
    static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);



	static float x(float phi, float theta) {
		return (cos(phi) * cos(theta));
	}
	static float x(float theta) {
		return (cos(theta));
	}

	static float y(float phi, float theta) {
		return (sin(phi));
	}

	static float z(float phi, float theta) {
		return (cos(phi) * sin(theta));
	}
	static float z(float theta) {
		return (sin(theta));
	}
};

#endif