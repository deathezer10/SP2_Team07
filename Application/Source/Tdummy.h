#ifndef TDUMMY_H
#define TDUMMY_H


#include "NPC.h"


class Tdummy : public NPC {


public:
	Tdummy(Scene* scene, Vector3 pos);
	~Tdummy();

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);

	 float _interactDistance;

	static unsigned TdummyCount;
	static Vector3* NearestTdummyPos;

private:
	float _currentaceleration = 60.0f;
	float _currentdeceleration = 60.0f;


	const float _MovementSpeed = 1.0f; // Movement speed
	const float _AttackDamage = 0.0f; // Damage per attack
	const float _DamageInterval = 0.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable

};

#endif