#include "Scene.h"
#include "D01.h"
#include "Missile.h"
#include "XF02.h"
#include "PlayerDataManager.h"

#include <map>


using std::multimap;



D01::D01(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(6000);
	type = Scene::GEO_D01;
	scale = 5.0f;

	toggleHealthbar(false);

	collider.setTrigger(true);
	setCollision(true);
	collider.setBoundingBoxSize(_defaultCollider);
}

D01::~D01() {
}

bool D01::update() {

	Vector3 bossToCamera = _scene->camera.playerView - position;

	// Deal damage if player touched the boss
	if (_currentState != AI_SKILL_THRUST && (_scene->_elapsedTime >= _ImmolateNextDamageTime) && collider.checkCollision(_scene->camera.getCollider()) == true) {
		PlayerDataManager::getInstance()->damagePlayer(_scene, _ImmolateDamage); // Do damage
		_ImmolateNextDamageTime = _scene->_elapsedTime + _ImmolateTick; // Cooldown
	}

	if (bossToCamera.LengthSquared() > (EnrageDistanceThreshold * EnrageDistanceThreshold)) {
		helperReset(); // Just in case we interupted the transition abrubtly
		rotationZ = 0;
		rotationX = 0;

		_currentState = AI_SKILL_MISSILE;
	}

	switch (_currentState) {

	case D01::AI_IDLE:

		// Cooldown in between skills
		if (_scene->_elapsedTime >= _NextSkillTime) {
			_currentState = static_cast<AI_STATE>(Math::RandInt() % AI_SKILL_TOTAL);
		}
		break;

	case D01::AI_SKILL_SUMMON:
		skill_summon();
		break;

	case D01::AI_SKILL_MISSILE:
		skill_missile();
		break;

	case D01::AI_SKILL_THRUST:
		skill_thrust();
		break;

	}
	//scaling the healthbar to health left in percentage
	if (hp > Math::EPSILON)//so that the healthbar do not scale to x axis to 0 and crash the program
	{
		hp = (float)currentHP / (float)defaultHP;
	}


	if (currentHP <= 0) {
		PlayerDataManager::getInstance()->getPlayerStats()->currency_earned += 1500;
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;
}

void D01::collisionHit(Vector3& hitPos) {
}


void D01::skill_summon() {

	// Create Summons
	if (_SummonSpawned < _SummonAmount && _scene->_elapsedTime >= _SummonNextSpawnTime) {
		++_SummonSpawned;
		Vector3 pos = position;
		pos.y += 5;

		XF02* m = new XF02(_scene, pos);
		m->getCollider().setTrigger(true);

		_scene->objBuilder.createObject(m, td_OBJ_TYPE::TYPE_ENEMY);

		_SummonNextSpawnTime = _scene->_elapsedTime + _SummonSpawnInterval; // Cooldown

	}

	// Reset everything once all are summoned
	if (_SummonSpawned >= _SummonAmount) {
		helperReset();
		_currentState = AI_IDLE;
		_NextSkillTime = _scene->_elapsedTime + _SkillSummonCooldown; // Add cooldown for the skill
	}

}

void D01::skill_missile() {

	// Create Missiles
	if (_MissileSpawned < _MissileAmount && _scene->_elapsedTime >= _MissileNextSpawnTime) {
		++_MissileSpawned;

		Missile* m = new Missile(_scene, position, &_scene->camera.playerView, _MissileHealth, _MissileDamage);

		_scene->objBuilder.createObject(m, td_OBJ_TYPE::TYPE_ENEMY);

		// Enrage if player is too far
		_MissileNextSpawnTime = _scene->_elapsedTime + 
			(((position - _scene->camera.position).Length() >= EnrageDistanceThreshold) ? _MissileEnrageSpawnInterval : _MissileSpawnInterval); 

	}

	// Reset everything once all missiles were fired
	if (_MissileSpawned >= _MissileAmount) {
		helperReset();
		_currentState = AI_IDLE;
		_NextSkillTime = _scene->_elapsedTime + _SkillMissileCooldown; // Add cooldown for the skill
	}


}

void D01::skill_thrust() {
	Vector3 bossToCamera = _scene->camera.playerView - position;

	// Deal damage if player touched the boss
	if ((_scene->_elapsedTime >= _ThrustNextDamageTime) && collider.checkCollision(_scene->camera.getCollider()) == true) {
		PlayerDataManager::getInstance()->damagePlayer(_scene, _ThrustDamage); // Do damage
		_ThrustNextDamageTime = _scene->_elapsedTime + _ThrustDamageInterval; // Cooldown
	}

	if (rotationX < 720) { // Initial spinning, to allow player to run

		// Face player first
		rotationY = -Math::RadianToDegree(atan2(bossToCamera.z, bossToCamera.x)) + 180;
		rotationZ = -Math::RadianToDegree(atan2(bossToCamera.y, bossToCamera.HorizontalLength()));

		// Spin the boss gradually
		_ThrustSpinVelocity += _ThrustSpinAcceleration * _scene->_dt;
		rotationX += _ThrustSpinVelocity * _scene->_dt;

	}
	else if (_ThrustHasTarget == false) { // Spinned enough, now lets select the target
		_ThrustHasTarget = true;
		_ThrustTarget = _scene->camera.position;

		Collider col(this, _thrustCollider.x, _thrustCollider.y, _thrustCollider.z);
		col.setTrigger(true);
		collider = col; // Increase collider size
	}

	// Do we have a target yet?
	if (_ThrustHasTarget) { // Charge forward until we reach the point
		Vector3 bossToTarget = _ThrustTarget - position;

		if (bossToTarget.Length() < 1) { // Reset everything once we reach the point
			rotationZ = 0;
			rotationX = 0;

			Collider col(this, _thrustCollider.x, _thrustCollider.y, _thrustCollider.z);
			col.setTrigger(true);
			collider = col; // Increase collider size

			_NextSkillTime = _scene->_elapsedTime + _SkillThrustCooldown; // Add cooldown for the skill

			helperReset();
			return;
		}

		rotationX += _ThrustSpinVelocity * _scene->_dt; // Still keep spinning
		position += bossToTarget.Normalized() * _ThrustVelocity * _scene->_dt; // Move towards the player		
	}


}

void D01::helperReset() {

	_currentState = AI_IDLE;

	_SummonSpawned = 0;

	_MissileSpawned = 0;

	_ThrustHasTarget = false;
	_ThrustSpinVelocity = 0; // reset spin velocity
	_ThrustNextDamageTime = 0;



}