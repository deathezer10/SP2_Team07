#ifndef SHAPE_H
#define SHAPE_H

#include "Mtx44.h"
#include "GL\glew.h"

class Shape
{

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_RECT = 0,
		GEO_TRIANGLE = 1,
		GEO_SQUARE = 2,
		NUM_GEOMETRY
	};

public:
	Shape();

	void prepareTriangle();
	void renderTriangle(Mtx44 translation, Mtx44 rotation, Mtx44 scale);

private:
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];

};


#endif 