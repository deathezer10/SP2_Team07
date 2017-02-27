#ifndef MISSILE_H
#define MISSILE_H

#include "NPC.h"


class Missile : public NPC {



public:
	Missile(Scene* scene, Vector3 pos, Vector3* target, bool isPlayer);
	~Missile();

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);




private:
	float _Acceleration = 50.0f;
	float _MaxVelocity = 500.0f;

	int _AttackDamage;

	Vector3* _MissileTarget;


};
#endif