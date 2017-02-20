#include "Scene.h"
#include "XF02.h"



unsigned XF02::XF02Count = 0;
Vector3* XF02::NearestXF02Pos = nullptr;



XF02::XF02(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(500);
	type = Scene::GEO_XF2;
	scale = 1.0f;
	_interactDistance = scale;
	isLightingEnabled = false;
	++XF02Count;
};

bool XF02::checkInteract() {

	if (NearestXF02Pos == nullptr) {
		NearestXF02Pos = &position;
	}

	Vector3 thisToCamera = (position - _scene->camera.playerView);
	Vector3 NearesXF02ToCamera = (*NearestXF02Pos) - _scene->camera.playerView;


	if (NearesXF02ToCamera.Length() > thisToCamera.Length()) {
		NearestXF02Pos = &position;
	}

	// Move XF02 towards player using the unit vector
	Vector3 distance = (position - _scene->camera.playerView);
	Vector3 unitDistance = distance.Normalized();

	// Rotate the XF02 towards the player
	if (distance.z > 0){
		rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x));
		rotationX = -(Math::RadianToDegree(atan2(distance.y, distance.z)));
	}
	else
	{
		rotationY = Math::RadianToDegree(atan2(distance.x, distance.z))+270;
		rotationX = -Math::RadianToDegree(atan2(distance.y, distance.z))+180;
	}
	// Move the XF02
	if (distance.Length() >= 40.0f) {
		_currentVelocity += _currentaceleration*_scene->_dt;

	}

	else if (distance.Length() <= 40.0f) {
		_currentVelocity = 0;


	}

	/*if (distance.Length() <= 20.0f) {
		_currentVelocity =-( _scene->camera.getCurrentVelocity() + 20.0f);

		}*/


	float moveX = unitDistance.x * _currentVelocity * _scene->_dt;
	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;
	float moveY = unitDistance.y  * _currentVelocity * _scene->_dt;

	position.x += -(moveX);
	position.z += -(moveZ);
	position.y += -(moveY);

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}


	return false;
}

void XF02::collisionHit(Vector3& hitPos) {

}