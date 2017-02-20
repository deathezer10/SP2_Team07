#include "PauseManager.h"
#include "Application.h"
#include "Scene.h"
#include "SceneManager.h"

#include "GLFW\glfw3.h"


PauseManager::PauseManager(Scene* scene){
	_scene = scene;
}

void PauseManager::UpdatePauseMenu(float dt){

	// Check for Key Up
	if (!Application::IsKeyPressed(PAUSE_KEY)) {
		_canPauseButtonPress = true;
	}

	// Check if pause button is pressed
	if (Application::IsKeyPressed(PAUSE_KEY) && _canPauseButtonPress == true) {
		_paused = !_paused; // Toggle between true and false
		_currentOption = 0;
		_scene->camera.ResetCursorVariables();
		_canPauseButtonPress = false;
	}

	// Nothing to do here since game is not paused
	if (_paused == false)
		return;

	if (!Application::IsKeyPressed(VK_UP) && !Application::IsKeyPressed(VK_DOWN)){
		_canMenuPress = true;
	}

	if (Application::IsKeyPressed(VK_UP) && _canMenuPress){
		--_currentOption;
		_canMenuPress = false;
	}

	if (Application::IsKeyPressed(VK_DOWN) && _canMenuPress){
		++_currentOption;
		_canMenuPress = false;
	}

	_currentOption = Math::Clamp<short>(_currentOption, 0, 3);


}

void PauseManager::RenderPauseMenu(){

	// Nothing to do here since game is not paused
	if (_paused == false)
		return;

	const Color white(1, 1, 1);

	string option1t = option1;
	string option2t = option2;
	string option3t = option3;
	string option4t = option4;

	switch (_currentOption) {

	case 0: // Resume
		option1t = ">" + option1 + "<";
		break;

	case 1: // Restart
		option2t = ">" + option2 + "<";
		break;

	case 2: // Main Menu
		option3t = ">" + option3 + "<";
		break;

	case 3: // Quit
		option4t = ">" + option4 + "<";
		break;

	}

	_scene->textManager.renderTextOnScreen(UIManager::Text{ title, white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option1t, white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option2t, white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option3t, white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", white, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option4t, white, UIManager::ANCHOR_CENTER_CENTER });

	if (Application::IsKeyPressed(VK_RETURN)){
		switch (_currentOption) {

		case 0: // Resume
			_paused = false;
			break;

		case 1: // Restart
			SceneManager::getInstance()->changeScene(_scene->createScene(_scene->sceneType)); // Restart the scene
			return;
			break;

		case 2: // Main Menu
			SceneManager::getInstance()->changeScene(_scene->createScene(Scene::TYPE_SCENE::SCENE_MAINMENU));
			break;

		case 3: // Quit
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
			break;

		}
	}

	_scene->textManager.reset();

}