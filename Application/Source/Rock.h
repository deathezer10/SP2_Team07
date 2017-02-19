#ifndef ROCK_H
#define ROCK_H


#include "Object.h"


// Interactable Rock, pickable and harvestable
class Rock : public Object {
	
public:
	Rock(Scene* scene, Vector3 pos);
	~Rock() { --RockCount; };

	static unsigned RockCount;

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);

	const float _interactDistance = 10.0f;

private:

};
#endif