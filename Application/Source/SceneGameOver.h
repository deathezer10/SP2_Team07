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
	enum TYPE_MENU {

		MENU_MAIN = 0,
		MENU_GAMEOVER,
		MENU_VICTORY

	};

	SceneGameover(TYPE_MENU type);
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
		
	Light light[1];

	Camera3 camera;

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	
	UIManager textManager;
	
	TYPE_MENU _menuType;
	unsigned _menuSelected;

};

#endif