#ifndef SLIME_H
#define SLIME_H


#include "NPC.h"


// Enemy Slime that moves towards player and damages him upon contact
class Slime : public NPC {
	
public:
	Slime(Scene* scene, Vector3 pos);
	~Slime() { slimeCount--; };

	virtual void checkInteract();
	virtual void interact();

	float currentHP = 5;

	const float _interactDistance = 2.5f;

	static unsigned slimeCount;

private:
	const float _MovementSpeed = 2.5f; // Movement speed
	const float _AttackDamage = 4.0f; // Damage per attack
	const float _DamageInterval = 1.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable
	
};
#endif