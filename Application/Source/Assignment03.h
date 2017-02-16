#ifndef ASSIGNMENT03_H
#define ASSIGNMENT03_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include <vector>

class Assignment03 : public Scene {

	friend class Object;
	friend class ObjectBuilder;
	friend class ObjectInteractor;
	friend class UIManager;
	friend class Bullet;

public:
	Assignment03();
	~Assignment03();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


private:
	Light light[2];

	void RenderSkybox();
	

};
#endif