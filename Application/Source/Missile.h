#ifndef MISSILE_H
#define MISSILE_H

#include "NPC.h"


class Missile : public NPC {



public:
	Missile(Scene* scene, Vector3 pos, NPC* target);
	Missile(Scene* scene, Vector3 pos, Vector3* target);
	~Missile() {};

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);




private:
	float _BossAcceleration = 10.0f;
	float _PlayerAcceleration = 50.0f;
	float _MaxVelocity = 500.0f;
	bool _IsPlayerMissile;

	int _AttackDamage;

	NPC* _MissileTarget = nullptr;
	Vector3* _MissileTargetPos;


};
#endif