#ifndef XF02_H
#define XF02_H


#include "NPC.h"


class XF02 : public NPC {

	enum AI_STATE {

		AI_IDLE = 0,
		AI_RETREAT,
		AI_CHASE,
		AI_ATTACK

	};

public:
	XF02(Scene* scene, Vector3 pos);
	~XF02();

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);

	static unsigned XF02Count;
	static Vector3* NearestXF02Pos;

private:
	float _Acceleration = 5.0f;
	float _MaxVelocity = 20.0f;

	const float _RetreatThreshold = 100; // Run away when HP is under this value
	const float _RetreatMaxDistance = 500; // Maximum distance that the enemy can run away
	const float _AttackMaxDistance = 50;
	const int _AttackDamage = 10; // Damage per attack
	const float _DamageInterval = 1.0f; // Cooldown between each attacks
	float _NextDamageTime = 0; // DamageInterval's helper variable
	
	AI_STATE _currentState = AI_CHASE;

};
#endif