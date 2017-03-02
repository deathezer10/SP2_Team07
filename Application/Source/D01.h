#ifndef D01_H
#define D01_H


#include "NPC.h"


#include <vector>



class D01 : public NPC {

	enum AI_STATE {

		AI_IDLE = 0,
		AI_SKILL_SUMMON,
		AI_SKILL_MISSILE,
		AI_SKILL_THRUST,
		AI_SKILL_TOTAL

	};

public:
	D01(Scene* scene, Vector3 pos);
	~D01();

	virtual bool update();
	virtual void collisionHit(Vector3& hitPos);

	static const int EnrageDistanceThreshold = 500; // If distance is above this, trigger Boss's enrage mode
	float hp = 1;
private:
	float _Acceleration = 5.0f;
	float _MaxVelocity = 20.0f;

	AI_STATE _currentState = AI_IDLE;

	const Vector3 _defaultCollider = Vector3(23.0f, 6.0f, 23.0f);
	const Vector3 _thrustCollider = Vector3(23.0f, 12.0f, 23.0f);

	void skill_summon();
	void skill_missile();
	void skill_thrust();

	// Misc Variables
	const float _SkillSummonCooldown = 5; // Summon cooldown
	const float _SkillThrustCooldown = 5; // Thrust cooldown
	const float _SkillMissileCooldown = 5; // Missile cooldown


	// Helpers
	float _NextSkillTime = 0;
	void helperReset(); // Reset all helper variables

	// Immolation variables
	float _ImmolateNextDamageTime = 0; // Helper
	const int _ImmolateDamage = 25; // Damage per tick
	const float _ImmolateTick = 1; // Damage tick cooldown


	// Summon variables
	float _SummonNextSpawnTime = 0;
	const float _SummonSpawnInterval = 4;
	const int _SummonAmount = 3;
	float _SummonSpawned = 0;


	// Missile variables
	float _MissileNextSpawnTime = 0;
	const float _MissileEnrageSpawnInterval = 1;
	const float _MissileSpawnInterval = 3;
	const int _MissileDamage = 75;
	const int _MissileHealth = 100;
	const int _MissileAmount = 3;
	float _MissileSpawned = 0;


	// Thrust variables
	Vector3 _ThrustTarget;
	bool _ThrustHasTarget = false;
	float _ThrustNextDamageTime = 0; // Helper
	float _ThrustSpinVelocity = 0; // Current Rotation velocity
	const float _ThrustSpinAcceleration = 90; // Spinning acceleration
	const float _ThrustVelocity = 100; // Constant speed that the boss moves at
	const float _ThrustDamageInterval = 1; // Cooldown between each damage tick
	const int _ThrustDamage = 100; // Damage per tick


};
#endif