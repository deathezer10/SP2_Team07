#ifndef SCENE_GAMEOVER_H
#define SCENE_GAMEOVER_H

#include "Scene.h"
#include "Light.h"

using std::string;

// This is a Game Menu Scene and not a GameOver Scene, was too late to refactor the Class name
class SceneGameover : public Scene {

public:
	enum TYPE_MENU {

		MENU_GAMEOVER = 0,
		MENU_VICTORY

	};

	SceneGameover(string title, TYPE_MENU type, TYPE_SCENE previousScene, int currencyEarned = 0);
	~SceneGameover();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


private:
	Light light[1];

	void RenderSkybox();

	string _title;

	unsigned _menuSelected;
	TYPE_MENU _menuType;
	TYPE_SCENE _previousScene;

	int _currencyEarned;

	bool canPressEnter = false;
	bool canPressUpDown = false;

};

#endif