#ifndef SceneCargoShip_H
#define SceneCargoShip_H

#include "Scene.h"
#include "Light.h"



struct PlayerSaveData;

class CargoShip;

class SceneCargoShip : public Scene {

public:
	SceneCargoShip();
	~SceneCargoShip();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Light light[2];

	void RenderSkybox();

	bool showDebugInfo = false;

	int currentObjective = 0;
	float _NextXF04SpawnTime = 0.0f;
	float _NextXF02SpawnTime = 0.0f;
	const float _SpawnXF02Interval = 40.0f;
	const float _SpawnXF04Interval = 20.0f;
	PlayerSaveData* pData;
	CargoShip* _CargoShip = nullptr;


};

#endif