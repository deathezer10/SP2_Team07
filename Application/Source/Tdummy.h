#ifndef TDUMMY_H
#define TDUMMY_H


#include "NPC.h"


// Enemy Slime that moves towards player and damages him upon contact
class Tdummy : public NPC {

public:
	Tdummy(Scene* scene, Vector3 pos);
	~Tdummy() { TdummyCount--; };

	virtual bool checkInteract();

	const float _interactDistance = 2.5f;

	static unsigned TdummyCount;

private:
	float _currentaceleration = 60.0f;
	float _currentdeceleration = 60.0f;


	const float _MovementSpeed = 1.0f; // Movement speed
	const float _AttackDamage = 0.0f; // Damage per attack
	const float _DamageInterval = 0.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable

};

#endif