#include "Scene.h"
#include "EnemyXF-04AI.h"
#include"CargoShip.h"

unsigned EnemyXF_04AI::EnemyXF_04AICount = 0;
Vector3* EnemyXF_04AI::NearestEnemyXF_04AIPos = nullptr;


//if the distance between the cargo and AI is >=40 the AI will move closer to the cargo
//if the AI <=40 than the cargo the AI will start shooting
//if the player <=50 to the cargo the enemy will move forward.
EnemyXF_04AI::EnemyXF_04AI(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(100);
	type = Scene::GEO_XF4;
	scale = 5.0f;
	//rotationZ = 90;
	_interactDistance = scale;
	isLightingEnabled = false;
	++EnemyXF_04AICount;
};

bool EnemyXF_04AI::checkInteract() {

	if (NearestEnemyXF_04AIPos == nullptr) {
		NearestEnemyXF_04AIPos = &position;
	}

	Vector3 thisToCamera = (position - _scene->camera.position);
	Vector3 NearesEnemyXF_04AIToCamera = (*NearestEnemyXF_04AIPos) - _scene->camera.position;


	if (NearesEnemyXF_04AIToCamera.Length() > thisToCamera.Length()) {
		NearestEnemyXF_04AIPos = &position;
	}

	// Move EnemyXF_04AI towards cargo using the unit vector
	Vector3 distance = (*CargoShip::NearestCargoShipPos - position);
	Vector3 unitDistance = distance.Normalized();
	//the distance between the player and the enemy
	Vector3 distance2 = (position-_scene->camera.position);

	// Rotate the EnemyXF_04AI towards the player
	rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) + 90;


	// Move the EnemyXF_04AI towards the cargo
	if (distance.Length() >= 40.0f) {
		//distance = *CargoShip::NearestCargoShipPos - position*_scene->_dt;
		_currentVelocity += _currentdeceleration*_scene->_dt;
	}
	else if (distance.Length()<=10)
	{
	//AI shoots bullet when it is near the cargo
		_currentVelocity=0;
		_scene->objBuilder.createObject(new Bullet(_scene, position, _AttackDamage, Vector3(rotationX, rotationY, rotationZ), unitDistance));
	}
	if (distance2.Length() <= 20)
	{
		//stop shooting and move away
		_currentVelocity -= _currentdeceleration*_scene->_dt;
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

void EnemyXF_04AI::collisionHit(Vector3& hitPos) {

}