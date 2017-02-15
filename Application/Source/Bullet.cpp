#include <iterator>
#include "Scene.h"
#include "Bullet.h"



Bullet::Bullet(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_NONE;

	scale = 0.4f;
	position.y -= 0.25f;

	_startingPosition = scene->camera.getPosition();
	_direction = scene->camera.getView().Normalized();

}

void Bullet::checkInteract() {

	position += _direction * _bulletSpeed * _scene->_dt;

	// Remove bullet once reached max distance
	if ((_startingPosition - position).Length() >= _bulletMaxDistance) {
		_scene->objBuilder.destroyObject(this);
		return;
	}

	vector<Object*> vec = _scene->objBuilder.objInteractor._objects;

	for (vector<Object*>::iterator it = vec.begin(); it != vec.end(); ++it) {

		// Rock collision
		if ((*it)->type == Scene::GEO_ROCK1 && !dynamic_cast<Rock*>((*it))->_isHarvested) {

			if (((*it)->position - position).Length() < _interactDistance) {
				(*it)->scale -= 2 * _scene->_dt; // Reduce size of rock when collided

				// Destroy the rock when it gets too small
				if ((*it)->scale <= 0.2f) {
					_scene->objBuilder.destroyObject(*it);
				}

				// _scene->objBuilder.destroyObject(this); // remove this bullet on collision
				return;
			}
		}
		else if ((*it)->type == Scene::GEO_SLIME) { // Slime collision

			if (((*it)->position - position).Length() < _interactDistance * 2) {

				// Reduce Slime's HP by 1 for every Bullet hit
				dynamic_cast<Slime*>(*it)->currentHP--;

				(*it)->scale -= 0.05f; // Scale down the slime

				_scene->objBuilder.destroyObject(this); // remove this bullet on collision
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
	_scene->modelStack.Rotate(rotationX, 1, 0, 0);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationZ, 0, 0, 1);
	_scene->modelStack.Scale(scale, scale, scale);

	_scene->RenderMesh(_scene->meshList[Scene::GEO_BULLET], true);

	_scene->modelStack.PopMatrix();

}