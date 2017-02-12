#include "shape.h"




void Shape::prepareTriangle(){

	// Triangle Vertex
	static const GLfloat vertex_buffer_data1[] = {
		-5.0f, 5.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		5.0f, 5.0f, 0.0f
	};

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE]);
	// Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data1), vertex_buffer_data1, GL_STATIC_DRAW);

	// Color
	static const GLfloat color_buffer_data1[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data1), color_buffer_data1, GL_STATIC_DRAW);

}

void Shape::renderTriangle(Mtx44 translation, Mtx44 rotation, Mtx44 scale){

}