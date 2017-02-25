#include <map>
#include <iterator>
#include "Scene.h"
#include "Bullet.h"
#include "NPC.h"
#include "PlayerDataManager.h"


using std::multimap;

// Player Bullet
Bullet::Bullet(Scene* scene, Vector3 pos, int damage) : Object(scene, pos + (scene->camera.getView().Normalized() * 4.5f)) {
	type = Scene::GEO_BULLET;

	_bulletDamage = damage;
	_bulletSpeed = (float)PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_speed;
	_bulletSpeed += scene->camera.getCurrentVelocity(); // bullet must be faster than the fighter!

	position.y -= 1.25f;

	rotationX = scene->camera.getPitch();
	rotationY = -scene->camera.getYaw() + 90;

	_direction = scene->camera.getView();
	_startingPosition = pos + _direction;

	setCollision(true);
	collider.setTrigger(true);
	collider.setBoundingBoxSize(Vector3(scale * 3, scale * 2, scale * 3));

}

// Enemy Bullet
Bullet::Bullet(Scene* scene, Vector3 pos, int damage, Vector3 rotation, Vector3 direction) : Object(scene, pos) {
	type = Scene::GEO_BULLET02;

	_isEnemyBullet = true;

	_bulletDamage = damage;
	_bulletSpeed = 75;

	position.y -= 1.0f;

	rotationX = rotation.x;
	rotationY = rotation.y;
	rotationZ = rotation.z;

	_direction = direction;
	_startingPosition = pos + _direction;

	setCollision(true);
	collider.setTrigger(true);
	collider.setBoundingBoxSize(Vector3(scale, scale, scale));

}

bool Bullet::update() {

	// Remove bullet once reached max distance
	if ((_startingPosition - position).LengthSquared() >= (_bulletMaxDistance * _bulletMaxDistance)) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	// Enemy Bullet
	if (_isEnemyBullet == true) {

		// Retrieve all values that from key 'Objective'
		auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_OBJECTIVE);

		for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

			Object* obj = it->second;

			// Objective bullet collision
			if (collider.checkCollision(obj->getCollider()) == true) {

				NPC* npc = static_cast<NPC*>(obj);

				// Damage the enemy and then remove this bullet
				npc->reduceHealth(_bulletDamage);

				_scene->objBuilder.destroyObject(this);

				return true;
			}

		}

		// Bullet hit the player?
		if (collider.checkCollision(_scene->camera.getCollider()) == true) {
			PlayerDataManager::getInstance()->damagePlayer(_bulletDamage);
			_scene->objBuilder.destroyObject(this);
			return true;
		}

	}
	else { // Player Bullet

		// Retrieve all values that from key 'Enemy'
		auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

		for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

			Object* obj = it->second;

			// NPC bullet collision
			if (collider.checkCollision(obj->getCollider()) == true) {

				NPC* npc = static_cast<NPC*>(obj);

				// Damage the enemy and then remove this bullet
				npc->reduceHealth(_bulletDamage);
				npc->reduceVelocity(npc->getCurrentVelocity() / 2);

				_scene->objBuilder.destroyObject(this);
				return true;
			}

		}
		
	}

	// Move bullet at the end so it doesn't "clip" through object in front of it
	position += _direction * _bulletSpeed * _scene->_dt;

	return false;
}

void Bullet::collisionHit(Vector3& hitPos) {

}