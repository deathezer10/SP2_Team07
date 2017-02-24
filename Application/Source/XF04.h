#ifndef EnemyXF_04AI_H
#define EnemyXF_04AI_H


#include "NPC.h"

/***************************************************
AUTHOR: LEE YONG HAN
**************************************************/
class XF04 : public NPC {

public:
	XF04(Scene* scene, Vector3 pos);
	~XF04();

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);

	float _interactDistance;

	static unsigned EnemyXF_04AICount;
	static Vector3* NearestEnemyXF_04AIPos;

private:
	const int _AttackDamage = 40; // Damage per attack
	const float _DamageInterval = 1.0f; // Cooldown between each attacks
	const float _AttackInterval = 5.f;

	float _AttackTime = 0;
	float _NextDamageTime = 0; // DamageInterval's helper variable

	float _Acceleration = 5.0f;
	float _MaxVelocity = 20.0f;

	const float _RetreatMaxDistanceAwayFromPlayer = 60;
	const float _RetreatMaxDistanceAwayFromCargo = 60;
	const float _RetreatFromPlayerThreshold = 30;

	bool wasRetreating = false;

};

#endif