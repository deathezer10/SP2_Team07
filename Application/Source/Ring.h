#ifndef RING_H
#define RING_H


#include "Object.h"


// Interactable RING, pickable and harvestable
class Ring : public Object {

public:
	Ring(Scene* scene, Vector3 pos);
	~Ring() { --RingCount; };

	static int RingCount;
	static Vector3* NearestRingPos;

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);

	float _interactDistance;

};
#endif