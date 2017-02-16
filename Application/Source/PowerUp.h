#ifndef POWERUP_H
#define POWERUP_H


#include "Object.h"


// Interactable RING, pickable and harvestable
class PowerUp : public Object {

public:
	PowerUp(Scene* scene, Vector3 pos,int powerType);
	~PowerUp() {};

	virtual void checkInteract();
	virtual void interact();
	virtual void render();
	static int RingCount;
	const float _interactDistance = 4.0f;

private:
	float _defaultScale = 1;
	int powertype;

};
#endif