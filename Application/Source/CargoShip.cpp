#include "Scene.h"
#include "CargoShip.h"



unsigned CargoShip::CargoShipCount = 0;
Vector3* CargoShip::NearestCargoShipPos = nullptr;



CargoShip::CargoShip(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(600);
	type = Scene::GEO_CARGOSHIP;
	scale = 5.0f;
	NearestCargoShipPos=&position;
};

bool CargoShip::checkInteract() {
	if (currentHP <= 0)
	{
		_scene->objBuilder.destroyObject(this);
	}
	return false;
}

void CargoShip::collisionHit(Vector3& hitPos) {

}