#include <map>
#include <iterator>
#include "Scene.h"
#include "Bullet.h"
#include "NPC.h"

using std::map;

Bullet::Bullet(Scene* scene, Vector3 pos) : Object(scene, pos + scene->camera.getView().Normalized() * 5) {
	type = Scene::GEO_NONE;

	position.y -= 1.0f;

	rotationZ = -scene->camera.getPitch();
	rotationY = -scene->camera.getYaw() - 90;

	_bulletSpeed += scene->camera.getCurrentVelocity();
	_direction = scene->camera.getView().Normalized();
	_startingPosition = pos + (_direction);

}

void Bullet::checkInteract() {

	position += _direction * _bulletSpeed * _scene->_dt;

	// Remove bullet once reached max distance
	if ((_startingPosition - position).Length() >= _bulletMaxDistance) {
		_scene->objBuilder.destroyObject(this);
		return;
	}

	auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

	for (map<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

		Object* temp = it->second;

		// NPC bullet collision
		if ((temp->position - position).Length() < _interactDistance) {

			NPC* npc = dynamic_cast<NPC*>(temp);
			Vector3 pushAway = (npc->position - _scene->camera.position).Normalized();
			pushAway *= 5; // multiply the unit vector by 5 so we can push him further

			// Damage the enemy and then remove this bullet
			//npc->position += pushAway;
			npc->reduceHealth(5);
			npc->reduceVelocity(10);
			_scene->objBuilder.destroyObject(this);
			return;
		}


	}

}

void Bullet::interact() {

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