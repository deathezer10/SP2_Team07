#include "Scene.h"
#include "Tdummy.h"



unsigned Tdummy::TdummyCount = 0;



Tdummy::Tdummy(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	type = Scene::GEO_TDUMMY;
	scale = 5.0f;
	TdummyCount++;
};

bool Tdummy::checkInteract() {

	// Move Tdummy towards player using the unit vector
	Vector3 distance = (position - _scene->camera.position);
	Vector3 unitDistance = distance.Normalized();

	// Rotate the Tdummy towards the player
	rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) + 90;


	// Move the Tdummy
	if (distance.Length() <= 30.0f) {
		_currentVelocity += _currentaceleration*_scene->_dt;

	}
	else if (distance.Length() <= 20.0f) {
		_currentVelocity = 60.0f;
	}
	else {
		if (_currentVelocity > 0) {
			_currentVelocity -= _currentdeceleration*_scene->_dt;
		}
		else if (_currentVelocity < 1) {
			_currentVelocity = 0;
		}
	}

	float moveX = unitDistance.x * _currentVelocity * _scene->_dt;
	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;
	float moveY = unitDistance.y  * _currentVelocity * _scene->_dt;

	position.x += moveX;
	position.z += moveZ;
	position.y += moveY;

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}


	return false;
}
