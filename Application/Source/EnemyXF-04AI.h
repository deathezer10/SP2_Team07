#ifndef EnemyXF_04AI_H
#define EnemyXF_04AI_H


#include "NPC.h"


class EnemyXF_04AI : public NPC {

public:
	EnemyXF_04AI(Scene* scene, Vector3 pos);
	~EnemyXF_04AI();

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);

	float _interactDistance;

	static unsigned EnemyXF_04AICount;
	static Vector3* NearestEnemyXF_04AIPos;

private:
	float _currentaceleration = 60.0f;
	float _currentdeceleration = 60.0f;


	const float _MovementSpeed = 1.0f; // Movement speed
	const float _AttackDamage = 10.0f; // Damage per attack
	const float _DamageInterval = 0.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable


	// Face the Arrow towards the target
	void RotateTowards(Vector3& target);

};

#endif