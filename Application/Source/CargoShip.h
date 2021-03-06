#ifndef CargoShip_H
#define CargoShip_H


#include "NPC.h"
#include "PlayerDataManager.h"

class PlayerDataManager;
class CargoShip : public NPC {

public:
	CargoShip(Scene* scene, Vector3 pos);
	~CargoShip() {};

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);
	
	float Destination = 1800.0f;
	float hp = 1;

	static Vector3* HyperPosition;

private:
	float _currentaceleration = 60.0f;
	float _currentdeceleration = 60.0f;
	float _maxvelocity = 10.0f;


	const float _MovementSpeed = 10.0f; // Movement speed
	const float _AttackDamage = 0.0f; // Damage per attack
	const float _DamageInterval = 0.0f; // Cooldown between each attacks

	float _NextDamageTime = 0; // DamageInterval's helper variable


	PlayerSaveData* pData;

};

#endif