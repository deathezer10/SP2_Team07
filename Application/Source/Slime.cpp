#include "Scene.h"
#include "Slime.h"



unsigned Slime::slimeCount = 0;



Slime::Slime(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	type = Scene::GEO_SLIME;
	scale = 0.5f;
	slimeCount++;
};

void Slime::checkInteract() {

	// Move slime towards player using the unit vector
	Vector3 distance = (position - _scene->camera.position);
	Vector3 unitDistance = distance.Normalized();

	float moveX = unitDistance.x * _MovementSpeed * _scene->_dt;
	float moveZ = unitDistance.z * _MovementSpeed * _scene->_dt;

	// Rotate the slime towards the player
	rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) - 90;

	// Move the Slime
	position.x -= moveX;
	position.z -= moveZ;

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return;
	}

	// Collided with player
	if ((position - _scene->camera.getPosition()).Length() <= _interactDistance && _scene->_elapsedTime >= _NextDamageTime) {
		_scene->textManager.queueRenderText(UIManager::Text("Ouch!", Color(1, 0, 0), UIManager::ANCHOR_CENTER_CENTER));
		_NextDamageTime = _scene->_elapsedTime + _DamageInterval;

	}

}

void Slime::interact() {}