#ifndef SCENE_GAMEOVER_H
#define SCENE_GAMEOVER_H

#include "Scene.h"
#include "Light.h"

using std::string;

// This is a Game Menu Scene and not a GameOver Scene, was too late to refactor the Class name
class SceneGameover : public Scene {

public:
	enum TYPE_MENU {

		MENU_MAIN = 0,
		MENU_GAMEOVER,
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

	TYPE_SCENE _previousScene;
	TYPE_MENU _menuType;
	unsigned _menuSelected;
	string _title;
	int _currencyEarned;

};

#endif