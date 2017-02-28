#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "Application.h"


class SceneManager{

    friend class Application;  // Allow Application to access Update()
    static SceneManager* _instance;

public:
    static SceneManager* getInstance(); 

    bool hasPendingScene(){
        return _hasPendingScene;
    }

    void changeScene(Scene* scene);
    
	Scene* getCurrentScene() { return currentScene; };
	    
protected:
    SceneManager(){};
	~SceneManager();

private:
    Scene* currentScene = nullptr;
	Scene* pendingScene = nullptr;

    bool _hasPendingScene = false;    

    StopWatch m_timer;

	void Update();
	void Exit();

};
#endif