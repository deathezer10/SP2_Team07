#include <map>
#include <iterator>
#include "Scene.h"
#include "Bullet.h"
#include "NPC.h"
#include "PlayerDataManager.h"


using std::multimap;

Bullet::Bullet(Scene* scene, Vector3 pos, int damage) : Object(scene, pos + scene->camera.getView().Normalized() * 5) {
	type = Scene::GEO_NONE;

	_bulletDamage = damage;
	_bulletSpeed = (float)PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_speed;
	_bulletSpeed += scene->camera.getCurrentVelocity(); // bullet must be faster than the fighter!

	position.y -= 1.0f;

	rotationZ = -scene->camera.getPitch();
	rotationY = -scene->camera.getYaw() - 90;

	_direction = scene->camera.getView();
	_startingPosition = pos + _direction;

}

Bullet::Bullet(Scene* scene, Vector3 pos, int damage, Vector3 enemyRotation, Vector3 direction) : Object(scene, pos) {
	type = Scene::GEO_NONE;

	_isEnemyBullet = true;

	_bulletDamage = damage;
	_bulletSpeed = 50;

	position.y -= 1.0f;

	rotationX = enemyRotation.x;
	rotationY = enemyRotation.y;
	rotationZ = enemyRotation.z;

	_direction = direction;
	_startingPosition = pos + _direction;

}

bool Bullet::checkInteract() {

	position += _direction * _bulletSpeed * _scene->_dt;

	// Remove bullet once reached max distance
	if ((_startingPosition - position).Length() >= _bulletMaxDistance) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	// Enemy Bullet
	if (_isEnemyBullet == true) {

		if ((_scene->camera.playerView - position).Length() < _interactDistance) {
			PlayerDataManager::getInstance()->damagePlayer(_bulletDamage);
			_scene->objBuilder.destroyObject(this);
			return true;
		}

	}
	else { // Player Bullet

		// Retrieve all values that from key 'Enemy'
		auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

		for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

			Object* temp = it->second;

			// NPC bullet collision
			if ((temp->position - position).Length() < _interactDistance) {

				NPC* npc = dynamic_cast<NPC*>(temp);
				Vector3 pushAway = (npc->position - _scene->camera.position).Normalized();
				pushAway *= 5; // multiply the unit vector by 5 so we can push him further

				// Damage the enemy and then remove this bullet
				//npc->position += pushAway;
				npc->reduceHealth(_bulletDamage);
				npc->reduceVelocity(npc->getCurrentVelocity() / 2);

				_scene->objBuilder.destroyObject(this);
				return true;
			}

		}

	}

	return false;
}

void Bullet::collisionHit(Vector3& hitPos) {

}

void Bullet::render() {

	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position.x, position.y, position.z);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationX, 0, 0, 1);
	_scene->modelStack.Rotate(rotationZ, 1, 0, 0);
	_scene->modelStack.Scale(scale, scale, scale);
	_scene->RenderMesh(_scene->meshList[Scene::GEO_BULLET], true);

	_scene->modelStack.PopMatrix();

}