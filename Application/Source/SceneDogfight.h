#ifndef SceneDogfight_H
#define SceneDogfight_H

#include "Scene.h"
#include "Light.h"



struct PlayerSaveData;


class SceneDogfight : public Scene {

public:
	SceneDogfight();
	~SceneDogfight();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Light light[2];

	void RenderSkybox();

	bool showDebugInfo = false;

	float _NextXF02SpawnTime = 0.0f;
	int currentObjective = 0;
	const float _SpawnXF02Interval = 40.0f;
	PlayerSaveData* pData;

};

#endif