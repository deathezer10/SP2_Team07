#ifndef ROCK_H
#define ROCK_H


#include "Object.h"
#include "NPC.h"

// Interactable Rock, pickable and harvestable
class Rock : public NPC {
	
public:
	Rock(Scene* scene, Vector3 pos);
	~Rock() { --RockCount; };

	static unsigned RockCount;

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);

private:
	const int _PlayerCollisionDamage = 100; // Deals this damage when player collided onto the asteroid



};
#endif