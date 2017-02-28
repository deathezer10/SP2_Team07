#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

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

class SceneMainMenu : public Scene {

public:
	enum TYPE_MENU {

		MENU_MAIN = 0,
		MENU_GAMEOVER,
		MENU_SHOP,
		MENU_VICTORY

	};

	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	
private:		
	Light light[1];	
		
	bool canPressUp = false;
	bool canPressDown = false;
	bool canPressEnter = false;
	bool isMenuOutside = true;
	float rotate = 0;
	float rotate_money = 0;
	float rotate_planet = 0;
	unsigned _outsideSelected = 0;
	unsigned _insideSelected = 0;

	PlayerSaveData* pData;

};

#endif