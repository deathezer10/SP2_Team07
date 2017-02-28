#ifndef SceneBoss_H
#define SceneBoss_H

#include "Scene.h"
#include "Light.h"



struct PlayerSaveData;

class D01;

class SceneBoss : public Scene {

public:
	SceneBoss();
	~SceneBoss();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Light light[2];

	void RenderSkybox();

	PlayerSaveData* pData;

	double currenttime = 361;

	int currentObjective = 0;

	D01* _Boss;

};

#endif