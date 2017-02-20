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
		collider(&position, boxWidth, boxHeight, boxDepth)
	{
	};

	virtual ~Object() {};

	// Process interaction logic every frame, returns true if Object is flagged for deletion
	virtual bool checkInteract() = 0;

	// Called when there's an collision with another Object
	// Arg0: Unit Vector of the opposing Object to this Object
	virtual void collisionHit(Vector3& hitPos) = 0;

	// Render this Object's mesh, can be overriden to render multiple meshes instead
	virtual void render();

	Vector3 position;
	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;
	float scale = 1;
	unsigned type;

	Collider& getCollider() { return collider; }

	// Is the Collider Enabled?
	bool& isCollidable() { return collidable; }

	// Toggle Collision checking for this Object
	void setCollision(bool enabled) { collidable = enabled; }

	Scene* _scene;

protected:
	Collider collider;
	bool collidable;
	bool isLightingEnabled = true;

};
#endif