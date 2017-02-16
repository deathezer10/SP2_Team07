#ifndef ROCK_H
#define ROCK_H


#include "Object.h"


// Interactable Rock, pickable and harvestable
class Rock : public Object {
	
public:
	Rock(Scene* scene, Vector3 pos);
	~Rock() {};

	virtual void checkInteract();
	virtual void interact();

	const float _interactDistance = 10.0f;

private:
	float _defaultScale = 1;

};
#endif