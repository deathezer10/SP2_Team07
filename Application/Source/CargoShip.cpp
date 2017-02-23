#include "Scene.h"
#include "CargoShip.h"



CargoShip* CargoShip::Instance = nullptr;

CargoShip::CargoShip(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(6000);
	type = Scene::GEO_CARGOSHIP;
	scale = 5.0f;
	rotationY = 90.f;
	_interactDistance = scale;

	collider.setBoundingBoxSize(Vector3(scale + 10, scale + 4, scale + 15));

	isLightingEnabled = false;
	Instance = this;
};

bool CargoShip::checkInteract() {

	Destination -= _currentVelocity * _scene->_dt;

	Vector3 distance = (position);//
	Vector3 unitDistance = distance.Normalized();


	// movement part
	if (Destination <= 0.0f)
	{
		_currentVelocity = 0;

	}
	else
	{
		_currentVelocity = _maxvelocity;
	}


	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;
	position.z += moveZ;




	//take damage part



	//scaling the healthbar to health left in percentage
	if (hp > Math::EPSILON)//so that the healthbar do not scale to x axis to 0 and crash the program
	{
		hp = (float)currentHP / (float)defaultHP;
		cargolife = (float)currentHP;
	}
 
	



	//destroyed part

	if (currentHP <= 0)
	{
		_scene->objBuilder.destroyObject(this);
		return true;
	}
	return false;
}

void CargoShip::collisionHit(Vector3& hitPos) {

}