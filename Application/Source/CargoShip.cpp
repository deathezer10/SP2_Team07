#include "Scene.h"
#include "CargoShip.h"



unsigned CargoShip::CargoShipCount = 0;
Vector3* CargoShip::NearestCargoShipPos = nullptr;
Vector3* CargoShip::CargoShipPos ;

float CargoShip::Destination = 1000.0f ;


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

	Destination -= _currentVelocity * _scene->_dt;

	Vector3 distance = (position);//
	Vector3 unitDistance = distance.Normalized();
	
	if (Destination!=0.0f)
	{
	_currentVelocity = _maxvelocity;
	}
	else
	{
		_currentVelocity = 0;
	}

	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;


	position.z += moveZ;
	
	_scene->textManager.queueRenderText(UIManager::Text{ std::to_string(Destination), Color(1, 0, 1), UIManager::ANCHOR_TOP_CENTER });//current distance left

	if (currentHP <= 0)
	{
		_scene->objBuilder.destroyObject(this);
	}
	return false;
}

void CargoShip::collisionHit(Vector3& hitPos) {

}