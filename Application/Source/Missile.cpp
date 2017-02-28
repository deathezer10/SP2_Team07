#include "Scene.h"
#include "Missile.h"
#include "PlayerDataManager.h"

#include <map>


using std::multimap;


Missile::Missile(Scene* scene, Vector3 pos, Vector3* target, bool isPlayer) : NPC(scene, pos, (isPlayer) ? false : true) {
	type = Scene::GEO_MISSILE;

	_MissileTarget = target;

	toggleHealthbar(false);
	setCollision(true);
	collider.setTrigger(true); // Don't knockback player
	collider.setBoundingBoxSize(Vector3(5, 5, 5));

	if (isPlayer) {
		_currentVelocity = Math::FAbs(scene->camera.getCurrentVelocity()); // Speed must be at least faster than player		
		_AttackDamage = PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage * 3; // Triple of the player's bullet damage
	}
	else {
		setHealth(50);
		_AttackDamage = 50; // Boss Damage
	}

}

Missile::~Missile() {
}


bool Missile::update() {

	// Accelerate the missile over time
	if (_currentVelocity < _MaxVelocity)
		_currentVelocity += _Acceleration * _scene->_dt;


	// Rotate towards target
	Vector3 missileToTarget = *_MissileTarget - position;
	rotationY = -Math::RadianToDegree(atan2(missileToTarget.z, missileToTarget.x)) + 180;
	rotationZ = -Math::RadianToDegree(atan2(missileToTarget.y, missileToTarget.HorizontalLength()));

	Vector3 unitDistance = missileToTarget.Normalized(); // unit distance to travel

	// Retrieve all values that from key 'Enemy'
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

	position.x += unitDistance.x * _currentVelocity * _scene->_dt;
	position.y += unitDistance.y * _currentVelocity * _scene->_dt;
	position.z += unitDistance.z * _currentVelocity * _scene->_dt;

	return false;
}

void Missile::collisionHit(Vector3& hitPos) {



}