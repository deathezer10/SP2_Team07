#ifndef ROCK_H
#define ROCK_H


#include "Object.h"


// Interactable Rock, pickable and harvestable
class Rock : public Object {
	
public:
	Rock(Assignment03* scene, Vector3 pos);
	~Rock() {};

	virtual void checkInteract();
	virtual void interact();

	const float _interactDistance = 3.0f;
	bool _isHarvested = false;

private:
	float _defaultScale = 1;

};
#endif