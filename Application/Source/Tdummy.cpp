#include "Scene.h"
#include "Tdummy.h"



unsigned Tdummy::TdummyCount = 0;
Vector3* Tdummy::NearestTdummyPos = nullptr;



Tdummy::Tdummy(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(100);
	type = Scene::GEO_TDUMMY;
	scale = 10.0f;
	_interactDistance = scale;
	isLightingEnabled = false;
	++TdummyCount;
};

Tdummy::~Tdummy() {
	 TdummyCount--; 

	 if (NearestTdummyPos == &position) {
		 NearestTdummyPos = &Vector3(0, 0, 0);
	 }
}

bool Tdummy::checkInteract() {

	if (NearestTdummyPos == nullptr) {
		NearestTdummyPos = &position;
	}

	Vector3 thisToCamera = (position - _scene->camera.position);
	Vector3 NearestDummyToCamera = (*NearestTdummyPos) - _scene->camera.position;


	if (NearestDummyToCamera.Length() > thisToCamera.Length()) {
		NearestTdummyPos = &position;
	}

	// Move Tdummy towards player using the unit vector
	Vector3 distance = (position - _scene->camera.position);
	Vector3 unitDistance = distance.Normalized();

	// Rotate the Tdummy towards the player


	rotationY = -Math::RadianToDegree(atan2(thisToCamera.z, thisToCamera.x)) + 180;
	rotationZ = -Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));


	// Move the Tdummy
	if (distance.Length() <= 40.0f) {
		_currentVelocity += _currentaceleration*_scene->_dt;

	}
	else if (distance.Length() <= 30.0f) {
		_currentVelocity = _scene->camera.getCurrentVelocity() + 20.0f;
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

void Tdummy::collisionHit(Vector3& hitPos) {

}