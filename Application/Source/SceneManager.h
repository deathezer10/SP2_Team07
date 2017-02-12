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
    
    
protected:
    SceneManager(){};
    ~SceneManager() { delete _instance; };

private:
    Scene* currentScene = 0;
    Scene* pendingScene = 0;

    bool _hasPendingScene = false;    

    StopWatch m_timer;

    void Update();

};
#endif