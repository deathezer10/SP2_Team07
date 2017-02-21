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
	_interactDistance = scale;
	isLightingEnabled = false;
	++EnemyXF_04AICount;
}

EnemyXF_04AI::~EnemyXF_04AI() {
	--EnemyXF_04AICount;
	if (NearestEnemyXF_04AIPos == &position) {
		NearestEnemyXF_04AIPos = &Vector3(0, 0, 0);
	}
}

bool EnemyXF_04AI::checkInteract() {


	if (NearestEnemyXF_04AIPos == nullptr) {
		NearestEnemyXF_04AIPos = &position;
	}

	Vector3 thisToCamera = (*CargoShip::NearestCargoShipPos - position); // Enemy to Cargo
	float thisToCameraLength = thisToCamera.Length(); // Length
	float thisToCameraHorizontalLength = thisToCamera.HorizontalLength(); // Horizontal Length
	Vector3 NearesXF04ToCamera = (*NearestEnemyXF_04AIPos) - _scene->camera.position;
	Vector3 EnemytoCamera = (position-_scene->camera.position);//Enemy to Camera

	if (NearesXF04ToCamera.LengthSquared() >= thisToCamera.LengthSquared()) {
		NearestEnemyXF_04AIPos = &position;
	}

	Vector3 unitDistance = thisToCamera.Normalized(); // Used to move towards or away from cargo
	Vector3 unitDistance2 = EnemytoCamera.Normalized();

	// Rotate towards cargo
	rotationY = -Math::RadianToDegree(atan2(thisToCamera.z, thisToCamera.x)) + 180;
	rotationZ = -Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));

	// Increment velocity every second
	if (_currentVelocity < _MaxVelocity)
		_currentVelocity += _Acceleration * _scene->_dt;

	// Determine the current AI's State
	if (EnemytoCamera.Length() < 20) {
		_currentState = AI_RETREAT;
	}
	else if (thisToCameraLength > 90) {
		_currentState = AI_CHASE;
	}
	else if (thisToCameraLength < 16) {
		_currentState = AI_ATTACK;
	}
	else {
		_currentState = AI_IDLE;
	}


	switch (_currentState) {

	case AI_STATE::AI_IDLE:
		break;

	case AI_STATE::AI_RETREAT:
		unitDistance2 *= 1; // Fly away from player
		rotationY -= 180; // Face away from player
		rotationZ = 0; // Since we're running, no need to look at player anymore
		switchstate = 1;
		// Stop running away if distance is too far from player
		if (EnemytoCamera.Length() >= 20) {
			unitDistance.SetZero();
		}
		break;

	case AI_STATE::AI_CHASE:
		// TODO: Insert checking of units beside this NPC to prevent 'converging'
		break;

	case AI_STATE::AI_ATTACK:

		float dirX = Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));

		if (_scene->_elapsedTime >= _NextDamageTime) {
			_scene->objBuilder.createObject(new Bullet(_scene, position, _AttackDamage, Vector3(dirX, rotationY + 90, 0), unitDistance));
			_NextDamageTime = _scene->_elapsedTime + _DamageInterval;
		}

		unitDistance.SetZero(); // Stay stationary while firing
		break;

	}
	switch (switchstate)
	{
	case 0:
			position.x += unitDistance.x * _currentVelocity * _scene->_dt;
			position.y += unitDistance.y  * _currentVelocity * _scene->_dt;
			position.z += unitDistance.z  * _currentVelocity * _scene->_dt;
			break;
	case 1:
		position.x += unitDistance2.x * _currentVelocity * _scene->_dt;
		position.y += unitDistance2.y  * _currentVelocity * _scene->_dt;
		position.z += unitDistance2.z  * _currentVelocity * _scene->_dt;
		break;
	}


	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;






	////////////////////////////////////LEARN FROM OLD CODE//////////////////////////////////////




	/*RotateTowards(_scene->camera.playerView);

	if (NearestEnemyXF_04AIPos == nullptr) {
		NearestEnemyXF_04AIPos = &position;
	}

	Vector3 thisToCamera = (position - _scene->camera.position);
	Vector3 NearesEnemyXF_04AIToCamera = (*NearestEnemyXF_04AIPos) - _scene->camera.position;


	if (NearesEnemyXF_04AIToCamera.Length() > thisToCamera.Length()) {
		NearestEnemyXF_04AIPos = &position;
	}*/


	///////////////////ABOVE DUNNID CHANGE///////////////////////////////////////////////////


	//// Move EnemyXF_04AI towards cargo using the unit vector
	//Vector3 distance = (*CargoShip::NearestCargoShipPos - position);
	//Vector3 unitDistance = distance.Normalized();
	////the distance between the player and the enemy
	//Vector3 distance2 = (position-_scene->camera.position);

	//// Rotate the EnemyXF_04AI towards the player
	//rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) + 90;


	//// Move the EnemyXF_04AI towards the cargo  
	//if (distance.Length() >= 40.0f) {
	//	//distance = *CargoShip::NearestCargoShipPos - position*_scene->_dt;
	//	_currentVelocity += _currentdeceleration*_scene->_dt;
	//}
	//else if (distance.Length()<=10)
	//{
	////AI shoots bullet when it is near the cargo
	//	_currentVelocity=0;
	//	float dirX = Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));
	//	if (_scene->_elapsedTime >= _NextDamageTime)
	//	{
	//		_scene->objBuilder.createObject(new Bullet(_scene, position, _AttackDamage, Vector3(dirX, rotationY + 90, 0), unitDistance));
	//		_NextDamageTime = _scene->_elapsedTime + _DamageInterval;
	//	}
	//}
	//if (distance2.Length() <= 20)
	//{
	//	//stop shooting and move away
	//	_currentVelocity -= _currentdeceleration*_scene->_dt;
	//}

	//float moveX = unitDistance.x * _currentVelocity * _scene->_dt;
	//float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;
	//float moveY = unitDistance.y  * _currentVelocity * _scene->_dt;

	//position.x += moveX;
	//position.z += moveZ;
	//position.y += moveY;

	//if (currentHP <= 0) {
	//	_scene->objBuilder.destroyObject(this);
	//	return true;
	//}


	//return false;
}

void EnemyXF_04AI::RotateTowards(Vector3& target) {
	
	Vector3 distance = (position - target);

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
}

void EnemyXF_04AI::collisionHit(Vector3& hitPos) {

}