#ifndef SceneTutorial_H
#define SceneTutorial_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CharacterManager.h"
#include "UIManager.h"
#include "ObjectBuilder.h"
#include "ObjectInteractor.h"

#include <vector>

class SceneTutorial : public Scene {

	friend class Object;
	friend class ObjectBuilder;
	friend class ObjectInteractor;
	friend class UIManager;
	friend class Bullet;

public:
	SceneTutorial(Application* app);
	~SceneTutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Application* _app;
	CharacterManager charManager;
	UIManager textManager;
	ObjectBuilder objBuilder;
	Camera3 camera;

private:
	Light light[2];
	

	void RenderCharacter();

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey);
	void RenderSkybox();

	const float shootCooldown = 0.05f; // Cooldown between each bullet shot

	bool pendingReset = false;
	float nextShootTime = 0;

};

#endif