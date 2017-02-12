#ifndef SCENE_GAMEOVER_H
#define SCENE_GAMEOVER_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "UIManager.h"

#include <vector>

class Application;

// This is a Game Menu Scene and not a GameOver Scene, was too late to refactor the Class name
class SceneGameover : public Scene {
	
public:
	enum GEOMETRY_TYPE {

		GEO_AXES,

		GEO_LIGHTBALL,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_TEXT,

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

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,

	};

	enum TYPE_MENU {

		MENU_MAIN = 0,
		MENU_GAMEOVER,
		MENU_VICTORY

	};

	SceneGameover(Application* app, TYPE_MENU type);
	~SceneGameover();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


private:
	double _dt;

	float translateX;
	float scaleAll;

	const float camera_width = 75;
	const float camera_height = 75;

	MS modelStack, viewStack, projectionStack;
	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	Mesh* meshList[NUM_GEOMETRY];
	
	Light light[1];

	Camera3 camera;

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	
	UIManager textManager;

	Application* _app;

	TYPE_MENU _menuType;
	unsigned _menuSelected;

};

#endif