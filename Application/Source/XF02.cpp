#include "Scene.h"
#include "XF02.h"



unsigned XF02::XF02Count = 0;
Vector3* XF02::NearestXF02Pos = nullptr;



XF02::XF02(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(200);
	type = Scene::GEO_XF2;
	scale = 10.0f;
	_interactDistance = scale * 2;
	isLightingEnabled = false;
	++XF02Count;
};

bool XF02::checkInteract() {

	if (NearestXF02Pos == nullptr) {
		NearestXF02Pos = &position;
	}

	Vector3 thisToCamera = (position - _scene->camera.position);
	Vector3 NearesXF02ToCamera = (*NearestXF02Pos) - _scene->camera.position;


	if (NearesXF02ToCamera.LengthSquared() >= thisToCamera.LengthSquared()) {
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
		rotationY = Math::RadianToDegree(atan2(distance.x, distance.z)) + 270;
		rotationX = -Math::RadianToDegree(atan2(distance.y, distance.z)) + 180;

	}

	// Move the XF02
	if (distance.Length() >= 10.0f && _currentVelocity <= 90.0f) //speed limit for enemy as well as to get enemy chase player
	{
		_currentVelocity += _currentaceleration*_scene->_dt;
	}
	if (distance.Length() <= 100.0f) //so that enemy do crash into player
	{
		_currentVelocity = 0;
	}
	if (distance.Length() <= 50.0f) //so that enemy do crash into player
	{
		_currentVelocity = -(_scene->camera.getCurrentVelocity() + 20.0f);
	}
	//if (distance.Length() <= 40.0f && currentHP <= 100)// get enemy to run when being chased
	//{
	//	_currentVelocity = -(_scene->camera.getCurrentVelocity() + 20.0f);
	//}
	if (currentHP <= 100)//get enemy to turn to running direction when chased
	{
		if (_currentVelocity >= -90.0f&&distance.Length() <= 500.0f)
		{
			_currentVelocity = -(_scene->camera.getCurrentVelocity() + 20.0f);
		}
		else
		{
			_currentVelocity = 0;
		}

		rotationY = _scene->camera.getYaw()*-1;

	}






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