#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"
#include "ObjectCollider.h"

class Scene;


// Abstract Base Class for the Scene's Interactable Objects
class Object {

public:
	Object(Scene* scene, Vector3 pos, bool colisionEnabled = false, float boxWidth = 5, float boxHeight = 5, float boxDepth = 5) :
		_scene(scene),
		position(pos),
		collidable(colisionEnabled),
		collider(&position, boxWidth, boxHeight, boxDepth) {
	};

	virtual ~Object() {};

	virtual void checkInteract() = 0;
	virtual void interact() = 0;
	virtual void render();

	Vector3 position;
	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;
	float scale = 1;
	unsigned type;

	Collider& getCollider() { return collider; }
	bool& isCollidable() { return collidable; }
	void setCollision(bool enabled) { collidable = enabled; }

	Scene* _scene;

protected:
	Collider collider;
	bool isInteracted = false;
	bool collidable;
};
#endif