#ifndef MISSILE_H
#define MISSILE_H

#include "NPC.h"


class Missile : public NPC {



public:
	// Player Missile
	Missile(Scene* scene, Vector3 pos, NPC* target);

	// Enemy Missile
	Missile(Scene* scene, Vector3 pos, Vector3* target, int health, int damage);
	~Missile() {};

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);
	virtual void render();
	

private:
	float _BossAcceleration = 10.0f;
	float _PlayerAcceleration = 50.0f;
	float _MaxVelocity = 500.0f;
	bool _IsPlayerMissile;

	int _AttackDamage;

	NPC* _MissileTarget = nullptr;
	Vector3* _MissileTargetPos;

	float _fakeRoll = 0; // used to spin the missile without spinning the hp bar

};
#endif