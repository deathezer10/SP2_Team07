#ifndef TDUMMY_H
#define TDUMMY_H


#include "Object.h"


// Enemy Slime that moves towards player and damages him upon contact
class Tdummy : public Object {

public:
	Tdummy(Scene* scene, Vector3 pos);
	~Tdummy() { TdummyCount--; };

	virtual void checkInteract();
	virtual void interact();

	float currentHP = 50;

	const float _interactDistance = 2.5f;

	static unsigned TdummyCount;

private:
	const float _MovementSpeed = 2.5f; // Movement speed
	const float _AttackDamage = 4.0f; // Damage per attack
	const float _DamageInterval = 1.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable

};