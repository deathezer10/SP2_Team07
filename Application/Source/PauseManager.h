#ifndef PAUSE_MANAGER_H
#define PAUSE_MANAGER_H

#include <string>


using std::string;


class Scene;


class PauseManager {


public:
	PauseManager(Scene* scene);
	~PauseManager(){};


	void UpdatePauseMenu(float dt);
	void RenderPauseMenu();

	bool isPaused(){ return _paused; };



private:
	Scene* _scene;
	bool _paused = false;
	short _currentOption = 0;
	const char PAUSE_KEY = '6'; // Default Value: 0x1B - VK_ESCAPE : Didn't want to include additional Header files
	bool _canPauseButtonPress = false;
	bool _canMenuPress = false;

	// Default Strings for the Pause Menu
	const string title = "Game Paused";
	const string option1 = "Resume";
	const string option2 = "Restart";
	const string option3 = "Main Menu";
	const string option4 = "Quit";

};
#endif