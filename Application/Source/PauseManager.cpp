#include "PauseManager.h"
#include "Application.h"
#include "Scene.h"
#include "SceneManager.h"
#include "MeshBuilder.h"

#include "GLFW\glfw3.h"


PauseManager::PauseManager(Scene* scene){
	_scene = scene;
}

void PauseManager::UpdatePauseMenu(float dt){

	GLFWwindow* window = glfwGetCurrentContext();

	// Check for Key Up
	if (!glfwGetKey(window, PAUSE_KEY)) {
		_canPauseButtonPress = true;
	}

	// Check if pause button is pressed
	if (glfwGetKey(window, PAUSE_KEY) && _canPauseButtonPress == true) {
		_paused = !_paused; // Toggle between true and false
		_currentOption = 0;
		_scene->camera.ResetCursorVariables();
		_canPauseButtonPress = false;
	}

	// Nothing to do here since game is not paused
	if (_paused == false)
		return;

	if (!glfwGetKey(window, GLFW_KEY_UP) && !glfwGetKey(window, GLFW_KEY_DOWN)){
		_canMenuPress = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) && _canMenuPress){
		--_currentOption;
		_canMenuPress = false;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) && _canMenuPress){
		++_currentOption;
		_canMenuPress = false;
	}

	_currentOption = Math::Clamp<short>(_currentOption, 0, 3);

	if (glfwGetKey(window, GLFW_KEY_ENTER)){
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
			return;
			break;

		case 3: // Quit
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
			break;

		}
	}

}

void PauseManager::RenderPauseMenu(){

	// Nothing to do here since game is not paused
	if (_paused == false)
		return;

	const Color color(0, 0, 0);

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

	glDisable(GL_DEPTH_TEST);
	_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_HP_BACKGROUND], ((float)Application::windowWidth() / 10) * 0.4f, ((float)Application::windowHeight() / 10) * 0.4f, Vector3(0, 0, 0), Vector3(10, 11, 1));
	glEnable(GL_DEPTH_TEST);
	_scene->textManager.renderTextOnScreen(UIManager::Text{ title, color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option1t, color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option2t, color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option3t, color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ "", color, UIManager::ANCHOR_CENTER_CENTER });
	_scene->textManager.renderTextOnScreen(UIManager::Text{ option4t, color, UIManager::ANCHOR_CENTER_CENTER });

	_scene->textManager.reset();

}