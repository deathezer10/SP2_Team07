#include "Scene.h"
#include "EnemyXF-04AI.h"
#include "CargoShip.h"

unsigned EnemyXF_04AI::EnemyXF_04AICount = 0;
Vector3* EnemyXF_04AI::NearestEnemyXF_04AIPos = nullptr;


//if the distance between the cargo and AI is >=40 the AI will move closer to the cargo
//if the AI <=40 than the cargo the AI will start shooting
//if the player <=50 to the cargo the enemy will move forward.
EnemyXF_04AI::EnemyXF_04AI(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(100);
	type = Scene::GEO_XF4;
	scale = 2.0f;
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

	Vector3 EnemyToCargo = (CargoShip::Instance->position - position); // Enemy to Cargo
	float EnemyToCargoLength = EnemyToCargo.Length(); // Length
	float EnemyToCargoHorizontalLength = EnemyToCargo.HorizontalLength(); // Horizontal Length
	Vector3 NearesXF04ToCamera = (*NearestEnemyXF_04AIPos) - _scene->camera.position;
	Vector3 EnemytoCamera = (position - _scene->camera.playerView);//Enemy to Camera

	if (NearesXF04ToCamera.LengthSquared() >= EnemyToCargo.LengthSquared()) {
		NearestEnemyXF_04AIPos = &position;
	}

	Vector3 unitDistance = EnemyToCargo.Normalized(); // Used to move towards or away from cargo
	Vector3 unitDistance2 = EnemytoCamera.Normalized();//Used to move towards or away from player

	// Rotate towards cargo
	rotationY = (-Math::RadianToDegree(atan2(EnemyToCargo.z, EnemyToCargo.x)) - 180);
	rotationZ = -Math::RadianToDegree(atan2(EnemyToCargo.y, EnemyToCargo.HorizontalLength()));

	// Increment velocity every second
	if (_currentVelocity < _MaxVelocity)
		_currentVelocity += _Acceleration * _scene->_dt;

	float dirX = Math::RadianToDegree(atan2(EnemyToCargo.y, EnemyToCargo.HorizontalLength()));


	if (wasRetreating == true){
		if (EnemytoCamera.Length() < _RetreatMaxDistanceAwayFromPlayer){//chase cargo if enemy and player is >60 distance
			rotationY = -Math::RadianToDegree(atan2(EnemytoCamera.z, EnemytoCamera.x)) + 180;
			rotationZ = 0;
			unitDistance = unitDistance2;
		}
		else {
			wasRetreating = false;
		}
	}
	else if (EnemytoCamera.Length() <  _RetreatFromPlayerThreshold){ // player is near, run away
		rotationY = -Math::RadianToDegree(atan2(EnemytoCamera.z, EnemytoCamera.x)) + 180;
		rotationZ = 0;
		unitDistance = unitDistance2;
		wasRetreating = true;
	}
	else if (EnemyToCargoLength < _RetreatMaxDistanceAwayFromCargo) { // chase cargo
		_currentVelocity -= _Acceleration * _scene->_dt * 2;

		if (_scene->_elapsedTime >= _NextDamageTime) { // attack cargo
			position.y += 1.f;
			_scene->objBuilder.createObject(new Bullet(_scene, position, _AttackDamage, Vector3(dirX, rotationY + 90, 0), unitDistance));
			_NextDamageTime = _scene->_elapsedTime + _DamageInterval;
			position.y -= 1.f;
		}
	}


	position.x += unitDistance.x * _currentVelocity * _scene->_dt;
	position.y += unitDistance.y  * _currentVelocity * _scene->_dt;
	position.z += unitDistance.z  * _currentVelocity * _scene->_dt;

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;
}

void EnemyXF_04AI::collisionHit(Vector3& hitPos) {

}