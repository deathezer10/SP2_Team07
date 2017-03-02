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
	float rotateangle = 0;
	PlayerSaveData* pData;
	float currenttime2 = 7;
	double currenttime = 361;

	int currentObjective = 0;
	float BossSpawnTime = 0.f;
	float BossSpawn = 2.f;
	bool IsBossSpawn = false;
	D01* _Boss;

	float warp_turn = 0;

};

#endif