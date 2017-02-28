#ifndef SCENE_H
#define SCENE_H


#include "Camera3.h"
#include "WayPoint.h"
#include "UIManager.h"
#include "SkillManager.h"
#include "PauseManager.h"
#include "ObjectBuilder.h"


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

		// UI Stuff
		GEO_TEXT,
		GEO_HP_BACKGROUND,
		GEO_HP_FOREGROUND,
		GEO_SHIELD_FOREGROUND,
		GEO_MENU_BACKGROUND,
		GEO_RADAR_BACKGROUND,
		GEO_RADAR_ENEMY,
		GEO_RADAR_PLAYER,
		GEO_DMGINDICATOR_RED,
		GEO_DMGINDICATOR_BLUE,

		// Scene Models
		GEO_BUILDING,
		GEO_DOOR,
		GEO_CRATE,
		GEO_LAMP,
		GEO_TABLE,
		GEO_CHAIR,
		GEO_SPACESHIP,
		GEO_SPACESTATION,

		// Space Fighter Meshes
		GEO_FG6,
		GEO_SF1,
		GEO_DF6,
		GEO_A10,
		GEO_XF2,
		GEO_XF4,
		GEO_D01,
		GEO_CARGOSHIP,
		GEO_MISSILE,

		GEO_SLIME,
		GEO_ROCK1,
		GEO_ROCK2,
		GEO_ROCK3,
		GEO_ROCK4,
		GEO_TDUMMY,
		GEO_BULLET,
		GEO_BULLET02,
		GEO_RING,
		GEO_CUBE,
		GEO_SPEED,
		GEO_BARRAGE,
		GEO_REGEN,
		GEO_WAYPOINT,

		//Main menu decoration
		GEO_MAIN_MENU,
		GEO_SINGLE_PLAYER,
		GEO_SINGLE_PLAYER_SELECTED,
		GEO_SHOP,
		GEO_SHOP_SELECTED,
		GEO_EXIT,
		GEO_EXIT_SELECTED,
		GEO_TITLE,
		GEO_MONEY,
		GEO_PLANET,
		GEO_HANGER,
		GEO_UI,
		GEO_GALAXY,

		// TRANSISITON SCREENS
		GEO_GAMEOVER,
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

	enum TYPE_SCENE {

		SCENE_TUTORIAL = 0,
		SCENE_MAINMENU,
		SCENE_SHOP,
		SCENE_GAMEOVER_VICTORY,
		SCENE_DOGFIGHT,
		SCENE_CARGOSHIP,
		SCENE_BOSS,

	};

	Scene(TYPE_SCENE type);
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	// Used for creating a 'new' Scene using TYPE_SCENE as the determinant. No initialization is done for the newly created Scene
	Scene* createScene(TYPE_SCENE type);

	MS modelStack, viewStack, projectionStack;

	Mesh* meshList[NUM_GEOMETRY];

	const TYPE_SCENE sceneType;

	float _dt;
	float _elapsedTime = 0;

	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;

	PauseManager pauseManager;
	SkillManager skillManager;
	UIManager textManager;
	ObjectBuilder objBuilder;
	Camera3 camera;
	WayPoint waypoint;

	void RenderMesh(Mesh *mesh, bool enableLight);

};

#endif