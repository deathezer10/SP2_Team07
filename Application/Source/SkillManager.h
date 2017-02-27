#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H



class Scene;
class Object;
struct PlayerStat;

class SkillManager {


public:
	SkillManager(Scene* scene);
	~SkillManager() {};

	void disableSkills() { skillEnabled = false; };
	void enableSkills() { skillEnabled = true; };

	// Processes skill logic every frame, should only be called in Update()
	void processSkills(double dt);


	// Reduces the Fighter's bullet interval by (arg0) for the given duration in Seconds
	void activateBarrage(float speed, float duration);

	// Set the Fighter's current speed to maximum for the given duration in Seconds
	void activateSpeedBoost(float duration);

private:
	Scene* _scene;
	PlayerStat* pStat;

	bool skillEnabled = true;
	bool canShoot = true;

	void processPowerUp();

	void processPassiveSkill();

	// Misc. Helper variables
	double _elapsedTime = 0;
	double _nextBulletShootTime = 0;
	double _nextSpeedBoostResetTime = 0;
	double _nextBarrageResetTime = 0;
	double _nextShieldRecoverTime = 0;

	float translateLeft = 0;
	float translateRight = 0;
	
	// Recovers Shield by this amount per second
	const float shieldRecoveryAmount = 10;
	bool isShieldRecovering = false;
	float shieldLastDamagedAmount = 0;

	bool isShootingRight = false;

	bool isBarrageActive = false;
	bool isSpeedBoostActive = false;
	bool isGodmodeActive = false;
	bool canGodmodePress = false;

	// Rocket Helper variables
	Object* lockedOnNPC = nullptr;
	const float rocketCooldownTime = 5; // Rocket cooldown time
	const float rocketTargetThreshold = 25.0f; // Only target NPC within this square cone of vision
	const float rocketTargetMaxSize = 10;
	const float rocketTargetMinSize = 1;
	float rocketTargetCurrentSize = rocketTargetMaxSize;
	float rocketNextShootTime = 0;
	bool isTargetFullyLocked = false;


};
#endif