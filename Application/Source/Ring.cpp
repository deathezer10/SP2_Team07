#include "Application.h"
#include "Scene.h"
#include "Ring.h"

int Ring::RingCount = 0;
Vector3* Ring::NearestRingPos = nullptr;


Ring::Ring(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_RING;
	scale = 20;
	_interactDistance = scale;
	++RingCount;
}

bool Ring::checkInteract() {

	if (NearestRingPos == nullptr) {
		NearestRingPos = &position;
	}

	Vector3 CurrentRingToCamera = (position - _scene->camera.position);
	Vector3 NearestRingToCamera = (*NearestRingPos) - _scene->camera.position;


	if (NearestRingToCamera.Length() > CurrentRingToCamera.Length()) {
		NearestRingPos = &position;
	}

	// Interacted
	if ((position - _scene->camera.position).Length() < _interactDistance) {
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;
}
void Ring::collisionHit(Vector3& hitPos) {
}