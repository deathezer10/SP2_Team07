#include "Scene.h"
#include "Tdummy.h"



unsigned Tdummy::TdummyCount = 0;



Tdummy::Tdummy(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_TDUMMY;
    scale = 5.0f;
	TdummyCount++;
};

void Tdummy::checkInteract() {

	// Move Tdummy towards player using the unit vector
	Vector3 distance = (position - _scene->camera.position);
	Vector3 unitDistance = distance.Normalized();

	// Rotate the Tdummy towards the player
	rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) +90;



	
	// Move the Tdummy
	if (distance.Length()<=30.0f)
	{
		_currentvelocity += _currentaceleration*_scene->_dt;

	}
	else if (distance.Length() <= 20.0f)
	{
		_currentvelocity =60.0f;

	}
	else 
	{
		if (_currentvelocity>0)
		{
			_currentvelocity -= _currentdeceleration*_scene->_dt;
		}
		else if (_currentvelocity<1)
		{
			_currentvelocity = 0;
		}
	}
	
	float moveX = unitDistance.x *_currentvelocity  * _scene->_dt;
	float moveZ = unitDistance.z  *_currentvelocity  * _scene->_dt;
	float moveY = unitDistance.y  *_currentvelocity  * _scene->_dt;


	position.x += moveX;
	position.z += moveZ;
	position.y += moveY;

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return;
	}

	// Collided with player
	/*if ((position - _scene->camera.getPosition()).Length() <= _interactDistance && _scene->_elapsedTime >= _NextDamageTime) {
		_scene->textManager.queueRenderText(UIManager::Text("Ouch!", Color(1, 0, 0), UIManager::ANCHOR_CENTER_CENTER));
		_scene->charManager.reduceHealth(_AttackDamage);
		_NextDamageTime = _scene->_elapsedTime + _DamageInterval;

	}*/

}

void Tdummy::interact() {}