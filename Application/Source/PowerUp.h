#ifndef POWERUP_H
#define POWERUP_H


#include "Object.h"


// Interactable RING, pickable and harvestable
class PowerUp : public Object {

public:
	enum PowerType {

		POWER_REGEN = 0,
		POWER_BARRAGE,
		POWER_SPEEDBOOST

	};

	PowerUp(Scene* scene, Vector3 pos, PowerType pType);
	~PowerUp() {};

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);
	virtual void render();
	
private:
	PowerType powertype;

};
#endif