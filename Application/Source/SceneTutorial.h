#ifndef SceneTutorial_H
#define SceneTutorial_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include <vector>

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
	
	void RenderCharacter();

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey);
	void RenderSkybox();

	const float shootCooldown = 0.05f; // Cooldown between each bullet shot

	bool pendingReset = false;
	float nextShootTime = 0;
	bool objectspawned = false;
	int currentObjective = 0;

};

#endif