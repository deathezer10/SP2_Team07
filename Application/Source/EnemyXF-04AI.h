#ifndef EnemyXF_04AI_H
#define EnemyXF_04AI_H


#include "NPC.h"


class EnemyXF_04AI : public NPC {
	enum AI_STATE {

		AI_IDLE = 0,
		AI_RETREAT,
		AI_CHASE,
		AI_ATTACK


	};
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
	const float _DamageInterval = 0.5f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable
	float _Acceleration = 5.0f;
	float _MaxVelocity = 20.0f;
	int switchstate = 0;
	const float _RetreatMaxDistance = 100;
	const float _RetreatThreshold = 100;
	AI_STATE _currentState = AI_CHASE;
	// Face the Arrow towards the target
	void RotateTowards(Vector3& target);

};

#endif