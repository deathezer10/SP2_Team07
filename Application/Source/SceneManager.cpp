#include "Application.h"
#include "SceneManager.h"

#include "PlayerDataManager.h"

#include <GLFW\glfw3.h>

SceneManager *SceneManager::_instance = nullptr;


SceneManager* SceneManager::getInstance(){
	if (!_instance){
		_instance = new SceneManager();
	}
	return _instance;
}

SceneManager::~SceneManager(){
}

void SceneManager::Exit(){
	delete _instance;
}

void SceneManager::Update(){

	m_timer.startTimer(); // Start timer to calculate how long it takes to render this frame

	GLFWwindow* window = glfwGetCurrentContext();

	while (!glfwWindowShouldClose(window))
	{
		// Change scene if there is any pending scene
		if (_hasPendingScene) {
			currentScene->Exit();
			delete currentScene;
			currentScene = pendingScene;
			currentScene->Init();
			_hasPendingScene = false;
			pendingScene = nullptr;
			std::cout << "Finished loading" << std::endl;
		}

		currentScene->Update(m_timer.getElapsedTime());
		currentScene->Render();
		//Swap buffers
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(Application::frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}

	currentScene->Exit();
	delete currentScene;

	// Cleaning up, chances are that the Application is terminating after this
	Exit();
	PlayerDataManager::getInstance()->Exit();
}

void SceneManager::changeScene(Scene* scene){
	if (!currentScene){
		currentScene = scene;
		scene->Init();
	}
	else {
		pendingScene = scene;
		_hasPendingScene = true;
		PlayerDataManager::getInstance()->ResetPlayerStats(); // Clear potential Power Up leakage before transitioning
	}
}