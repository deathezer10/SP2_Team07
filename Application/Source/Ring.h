#ifndef RING_H
#define RING_H


#include "Object.h"


// Interactable RING, pickable and harvestable
class Ring : public Object {

public:
	Ring(Scene* scene, Vector3 pos);
	~Ring() {};

	virtual void checkInteract();
	virtual void interact();

	const float _interactDistance = 3.0f;

private:
	float _defaultScale = 1;

};
#endif