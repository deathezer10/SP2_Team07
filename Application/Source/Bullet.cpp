#include <map>
#include <iterator>
#include "Scene.h"
#include "Bullet.h"

using std::map;

Bullet::Bullet(Scene* scene, Vector3 pos) : Object(scene, pos + scene->camera.getView().Normalized() * 5) {
	type = Scene::GEO_NONE;

	position.y -= 0.5f;

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

	std::multimap<td_OBJ_TYPE, Object*> vec = _scene->objBuilder.objInteractor._objects;

	for (map<td_OBJ_TYPE, Object*>::iterator it = vec.begin(); it != vec.end(); ++it) {

		Object* temp = it->second;

		// Rock collision
		if (temp->type == Scene::GEO_ROCK1 && !dynamic_cast<Rock*>(temp)->_isHarvested) {

			if ((temp->position - position).Length() < _interactDistance) {
				temp->scale -= 2 * _scene->_dt; // Reduce size of rock when collided

				// Destroy the rock when it gets too small
				if (temp->scale <= 0.2f) {
					_scene->objBuilder.destroyObject(temp);
				}

				// _scene->objBuilder.destroyObject(this); // remove this bullet on collision
				return;
			}
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