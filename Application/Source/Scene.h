#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include "MatrixStack.h"

class Scene {
public:
	enum GEOMETRY_TYPE {

		GEO_NONE,

		GEO_AXES,
		GEO_FLOOR,

		GEO_LIGHTBALL,
		GEO_LIGHTBALL2,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_TEXT,

		// Scene Models
		GEO_BUILDING,
		GEO_DOOR,
		GEO_CRATE,
		GEO_LAMP,
		GEO_TABLE,
		GEO_CHAIR,
		GEO_SPACESHIP,
		GEO_SLIME,
		GEO_ROCK1,

		GEO_BULLET,

		NUM_GEOMETRY
	};

	enum UNIFORM_TYPE {

		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,

	};

	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	
	MS modelStack, viewStack, projectionStack;

	Mesh* meshList[NUM_GEOMETRY];
	
	float _dt;
	float _elapsedTime = 0;

	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;

};

#endif