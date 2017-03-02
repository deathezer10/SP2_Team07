#include "Scene.h"
#include "Missile.h"
#include "PlayerDataManager.h"

#include <map>
#include "GL\glew.h"


using std::multimap;


// Player Missile
Missile::Missile(Scene* scene, Vector3 pos, NPC* target) : NPC(scene, pos, false) {
	type = Scene::GEO_MISSILE;

	_MissileTarget = target;
	_MissileTargetPos = &target->position;

	_IsPlayerMissile = true;

	toggleHealthbar(false);
	setCollision(true);
	collider.setTrigger(true); // Don't knockback player
	collider.setBoundingBoxSize(Vector3(5, 5, 5));

	_currentVelocity = Math::FAbs(scene->camera.getCurrentVelocity()); // Speed must be at least faster than player		
	_AttackDamage = PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage * 5; // Triple of the player's bullet damage
}

// Boss Missile
Missile::Missile(Scene* scene, Vector3 pos, Vector3* playerPosition, int health,int damage) : NPC(scene, pos, true) {
	type = Scene::GEO_MISSILE;

	_MissileTargetPos = playerPosition;

	_IsPlayerMissile = false;

	scale = 2;

	setCollision(true);
	collider.setTrigger(true); // Don't knockback player
	collider.setBoundingBoxSize(Vector3(5,5,5));

	setHealth(health);
	_currentVelocity = 0;
	_AttackDamage = damage; // Boss Damage
}

bool Missile::update() {

	if (_IsPlayerMissile == true) {
		if (_MissileTarget->getCurrentHealth() <= 0) { // Enemy died before missile reached
			_scene->objBuilder.destroyObject(this);
			return true;
		}
	}
	else {
		if (currentHP <= 0) { // Player destroyed missile
			_scene->objBuilder.destroyObject(this);
			return true;
		}
	}

	// Accelerate the missile over time
	if (_currentVelocity < _MaxVelocity) {
		_currentVelocity += ((_IsPlayerMissile == true) ? _PlayerAcceleration : _BossAcceleration) * _scene->_dt;
	}

	// Rotate towards target
	Vector3 missileToTarget = *_MissileTargetPos - position;
	rotationY = -Math::RadianToDegree(atan2(missileToTarget.z, missileToTarget.x)) + 180;
	rotationZ = -Math::RadianToDegree(atan2(missileToTarget.y, missileToTarget.HorizontalLength()));
	_fakeRoll += 720 * _scene->_dt; // spin every frame

	Vector3 unitDistance = missileToTarget.Normalized(); // unit distance to travel

	// Retrieve all values that from key 'Enemy'
	if (_IsPlayerMissile) { // Player missile
		auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

		for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

			Object* obj = it->second;

			// NPC bullet collision
			if (collider.checkCollision(obj->getCollider()) == true) {

				NPC* npc = static_cast<NPC*>(obj);

				// Damage the enemy and then remove this missile
				npc->reduceHealth(_AttackDamage);

				_scene->objBuilder.destroyObject(this);
				return true;
			}

		}
	}
	else { // Enemy missile
		if (collider.checkCollision(_scene->camera.getCollider()) == true) {
			PlayerDataManager::getInstance()->damagePlayer(_scene, _AttackDamage);
			_scene->objBuilder.destroyObject(this);
			return true;
		}
	}


	position.x += unitDistance.x * _currentVelocity * _scene->_dt;
	position.y += unitDistance.y * _currentVelocity * _scene->_dt;
	position.z += unitDistance.z * _currentVelocity * _scene->_dt;

	return false;
}

void Missile::collisionHit(Vector3& hitPos) {
}

void Missile::render() {
	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position.x, position.y, position.z);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationZ, 0, 0, 1);
	_scene->modelStack.Rotate(rotationX, 1, 0, 0);
	_scene->modelStack.Scale(scale, scale, scale);
	_scene->modelStack.PushMatrix();
	_scene->modelStack.Rotate(_fakeRoll, 1, 0,0);
	_scene->RenderMesh(_scene->meshList[type], isLightingEnabled);
	_scene->modelStack.PopMatrix();

	if (_isHealthBarEnabled) {

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		const float overallSize = 0.5f;
		float barSize = ((float)currentHP / (float)defaultHP) * overallSize;

		// Background
		_scene->modelStack.PushMatrix();
		{
			_scene->modelStack.Translate(0, 0.25f, -overallSize);
			_scene->modelStack.Rotate(-90, 0, 1, 0);
			_scene->modelStack.Scale(overallSize, 0.025f, 0.1f);
			_scene->RenderMesh(_scene->meshList[Scene::GEO_HP_BACKGROUND], false);
		}
		_scene->modelStack.PopMatrix();

		// Foreground
		_scene->modelStack.PushMatrix();
		{
			_scene->modelStack.Translate(0, 0.25f, -barSize);
			_scene->modelStack.Rotate(-90, 0, 1, 0);
			_scene->modelStack.Scale(barSize, 0.025f, 0.1f);
			_scene->RenderMesh(_scene->meshList[Scene::GEO_HP_FOREGROUND], false);
		}
		_scene->modelStack.PopMatrix();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}

	_scene->modelStack.PopMatrix();


}