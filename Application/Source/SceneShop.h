#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "UIManager.h"

#include "PlayerDataManager.h"

#include <vector>

class Application;
class PlayerDataManager;

class SceneShop : public Scene {

public:
	SceneShop();
	~SceneShop();

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

	bool canChangeMenu = false;
	bool canChangeMenu2 = false;
	bool canChangeMenuBack = false;

	Light light[1];

	

	void RenderSkybox();

	

	PlayerData* pData;

	Application* _app;

	unsigned _menuSelected;

	bool ChangeShip;
	int column = 0;
	int row = 0;
	bool CanPress;



};

#endif