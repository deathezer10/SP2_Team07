#include "Scene.h"
#include "D01.h"
#include "CargoShip.h"
#include "PlayerDataManager.h"
#include "SceneDogfight.h"

#include <map>

using std::multimap;


unsigned D01::D01Count = 0;
Vector3* D01::NearestD01Pos = nullptr;


D01::D01(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(6000);
	type = Scene::GEO_D01;
	scale = 1.0f;
	isLightingEnabled = false;
	++D01Count;

	setCollision(true);
	collider.setBoundingBoxSize(Vector3(6.0f, 3.0f, 6.0f));
}

D01::~D01() {
	--D01Count;

	SceneDogfight::killcount += 1;

	if (NearestD01Pos == &position) {
		NearestD01Pos = &Vector3(0, 0, 0);
	}
}

bool D01::update() {

	if (NearestD01Pos == nullptr) {
		NearestD01Pos = &position;
	}

	Vector3 thisToCamera = (_scene->camera.playerView - position); // Enemy to Camera
	float thisToCameraLength = thisToCamera.Length(); // Length
	float thisToCameraHorizontalLength = thisToCamera.HorizontalLength(); // Horizontal Length
	Vector3 NearesD01ToCamera = (*NearestD01Pos) - _scene->camera.position;

	if (NearesD01ToCamera.LengthSquared() >= thisToCamera.LengthSquared()) {
		NearestD01Pos = &position;
	}

	Vector3 unitDistance = thisToCamera.Normalized(); // Used to move towards or away from player


	// Rotate towards player
	rotationY = -Math::RadianToDegree(atan2(thisToCamera.z, thisToCamera.x)) + 180;
	rotationZ = -Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));

	// Increment velocity every second
	if (_currentVelocity < _MaxVelocity)
		_currentVelocity += _Acceleration * _scene->_dt;

	// Determine the current AI's State
	if (currentHP < _RetreatThreshold) {
		_currentState = AI_RETREAT;
	}
	else if (thisToCameraLength > 90) {
		_currentState = AI_CHASE;
	}
	else if (thisToCameraLength <= 90) {
		_currentState = AI_ATTACK;
	}
	else {
		_currentState = AI_IDLE;
	}


	switch (_currentState) {

	case AI_STATE::AI_IDLE:
		break;

	case AI_STATE::AI_RETREAT:

		rotationY -= 180; // Face away from player
		rotationZ = 0; // Since we're running, no need to look at player anymore

		// Stop running away if distance is too far from player
		if (thisToCameraHorizontalLength >= _RetreatMaxDistance) {
			unitDistance.SetZero();
		}
		else {
			unitDistance *= -1; // Fly away from player
		}
		break;

	case AI_STATE::AI_CHASE:
		// TODO: Insert checking of units beside this NPC to prevent 'converging'

		if (thisToCameraHorizontalLength >= _AttackMaxDistance) {
			unitDistance *= 2;
		}
		else {
			unitDistance.SetZero();
		}

		break;

	case AI_STATE::AI_ATTACK:

		float dirX = Math::RadianToDegree(atan2(thisToCamera.y, thisToCamera.HorizontalLength()));

		if (thisToCameraHorizontalLength <= _AttackMaxDistance) {
			if (_scene->_elapsedTime >= _NextDamageTime) {
				_scene->objBuilder.createObject(new Bullet(_scene, position, _AttackDamage, Vector3(dirX, rotationY + 90, 0), unitDistance));
				_NextDamageTime = _scene->_elapsedTime + _DamageInterval;
			}

			unitDistance.SetZero(); // Stay stationary while firing
		}

		break;

	}

	// Prevent this unit from merging with others
	auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

	for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

		Object* obj = it->second;
		Vector3 hitDir;

		// Detect collision for all other enemies
		if (obj != this && collider.checkCollision(obj->getCollider(), &hitDir) == true) {
			position += -hitDir * _scene->_dt;
		}

	}

	position.x += unitDistance.x * _currentVelocity * _scene->_dt;
	position.y += unitDistance.y  * _currentVelocity * _scene->_dt;
	position.z += unitDistance.z  * _currentVelocity * _scene->_dt;

	if (currentHP <= 0) {
		PlayerDataManager::getInstance()->getPlayerStats()->currency_earned += 50;
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;
}

void D01::collisionHit(Vector3& hitPos) {

}