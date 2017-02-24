#include "Application.h"
#include "SceneManager.h"

#include "PlayerDataManager.h"

#include <GLFW\glfw3.h>

SceneManager *SceneManager::_instance = 0;


SceneManager* SceneManager::getInstance(){
    if (!_instance){
        _instance = new SceneManager();
    }
    return _instance;
}


void SceneManager::Update(){

    m_timer.startTimer(); // Start timer to calculate how long it takes to render this frame

	//while (!glfwWindowShouldClose(glfwGetCurrentContext()) && !Application::IsKeyPressed(VK_ESCAPE)) orginal code for if statement below in case needed

    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        currentScene->Update(m_timer.getElapsedTime());
        currentScene->Render();
        //Swap buffers
        glfwSwapBuffers(glfwGetCurrentContext());
        //Get and organize events, like keyboard and mouse input, window resizing, etc...
        glfwPollEvents();
        m_timer.waitUntil(Application::frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

        // Change scene if there is any pending scene
        if (_hasPendingScene) {
            currentScene->Exit();
            delete currentScene;
            currentScene = pendingScene;
            currentScene->Init();
            _hasPendingScene = false;
            pendingScene = nullptr;
        }
    } //Check if the ESC key had been pressed or if the window had been closed

    currentScene->Exit();
    delete currentScene;

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