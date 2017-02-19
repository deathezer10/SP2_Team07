#ifndef SceneTutorial_H
#define SceneTutorial_H

#include "Scene.h"
#include "Light.h"



struct PlayerSaveData;


class SceneTutorial : public Scene {
	
public:
	SceneTutorial();
	~SceneTutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	
private:
	Light light[2];
	
	void RenderSkybox();
	
	bool showDebugInfo = false;

	int currentObjective = 0;

	PlayerSaveData* pData;

};

#endif