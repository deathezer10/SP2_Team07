#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

class Scene;

class SkillManager {


public:
	SkillManager(Scene* scene) { _scene = scene; };
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

	bool skillEnabled = true;
	bool canShoot = true;

	void processPowerUp();

	void processPassiveSkill();

	// Helper variables
	double _elapsedTime = 0;
	double _nextShootTime = 0;
	double _nextSpeedBoostResetTime = 0;
	double _nextBarrageResetTime = 0;
	double _nextShieldRecoverTime = 0;

	// Recovers Shield by this amount per second
	const float shieldRecoveryAmount = 10;
	bool isShieldRecovering = false;
	float shieldLastDamagedAmount = 0;

	bool isBarrageActive = false;
	bool isSpeedBoostActive = false;

};
#endif