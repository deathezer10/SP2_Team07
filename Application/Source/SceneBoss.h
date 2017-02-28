#ifndef SceneBoss_H
#define SceneBoss_H

#include "Scene.h"
#include "Light.h"



struct PlayerSaveData;


class SceneBoss : public Scene {

public:
	SceneBoss();
	~SceneBoss();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	double currenttime = 361;
	static unsigned killcount;

private:
	Light light[2];

	void RenderSkybox();
	unsigned int _maxKillcount = 20;
	bool showDebugInfo = false;

	float _NextXF02SpawnTime = 0.0f;
	int currentObjective = 0;
	const float _SpawnXF02Interval = 12.0f;
	PlayerSaveData* pData;

};

#endif