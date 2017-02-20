#ifndef XF02_H
#define XF02_H


#include "NPC.h"


class XF02 : public NPC {

public:
	XF02(Scene* scene, Vector3 pos);
	~XF02() { --XF02Count; };

	virtual bool checkInteract();
	virtual void collisionHit(Vector3& hitPos);

	float _interactDistance;

	static unsigned XF02Count;
	static Vector3* NearestXF02Pos;

private:
	float _currentaceleration = 10.0f;
	float _currentdeceleration = 10.0f;


	const float _MovementSpeed = 1.0f; // Movement speed
	const float _AttackDamage = 0.0f; // Damage per attack
	const float _DamageInterval = 0.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable

};

#endif