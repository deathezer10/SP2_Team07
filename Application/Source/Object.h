#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"


class Scene;


// Abstract Base Class for the Scene's Interactable Objects
class Object {

public:
	Object(Scene* scene, Vector3 pos, bool colisionEnabled = true, float boxWidth = 5, float boxHeight = 5, float boxDepth = 5) :
		_scene(scene), position(pos),
		bboxWidth(boxWidth),
		bboxHeight(boxHeight),
		bboxDepth(boxDepth)	{};

	virtual ~Object() {};

	virtual void checkInteract() = 0;
	virtual void interact() = 0;
	virtual void render();
	bool checkCollision(Vector3 &other, Vector3* hitDirection);

	Vector3 position;
	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;
	float scale = 1;
	unsigned type;

	float bboxWidth;
	float bboxHeight;
	float bboxDepth;

protected:
	Scene* _scene;
	bool isInteracted = false;

};
#endif