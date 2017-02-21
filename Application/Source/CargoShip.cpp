#include "Scene.h"
#include "CargoShip.h"



unsigned CargoShip::CargoShipCount = 0;
Vector3* CargoShip::NearestCargoShipPos = nullptr;



CargoShip::CargoShip(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(600);
	type = Scene::GEO_CARGOSHIP;
	scale = 5.0f;
	rotationY = 90.f;
	_interactDistance = scale;
	NearestCargoShipPos=&position;
	isLightingEnabled = false;
};

bool CargoShip::checkInteract() {

	Vector3 distance = (position - 300.0f);
	Vector3 unitDistance = distance.Normalized();

	if (distance.Length() <= 150.0f) {
		_currentVelocity += _MovementSpeed*_scene->_dt;

	}

	
	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;


	if (position.z<15000.0f)
	{
		position.z += 10.0f;

	}
	

	if (currentHP <= 0)
	{
		_scene->objBuilder.destroyObject(this);
	}
	return false;
}

void CargoShip::collisionHit(Vector3& hitPos) {

}