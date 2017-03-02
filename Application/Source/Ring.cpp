#include "Application.h"
#include "Scene.h"
#include "Ring.h"

int Ring::RingCount = 0;
Vector3* Ring::NearestRingPos = nullptr;


Ring::Ring(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_RING;
	scale = 20;
	++RingCount;

	setCollision(true);
	collider.setBoundingBoxSize(Vector3(scale * 2, scale * 2, scale));
}

Ring::~Ring() {
	--RingCount;
	if (NearestRingPos == &position) {
		NearestRingPos = &Vector3(0, 0, 0);
	}
};

bool Ring::update() {

	if (NearestRingPos == nullptr) {
		NearestRingPos = &position;
	}

	Vector3 CurrentRingToCamera = (position - _scene->camera.position);
	Vector3 NearestRingToCamera = (*NearestRingPos) - _scene->camera.position;


	if (NearestRingToCamera.LengthSquared() > CurrentRingToCamera.LengthSquared()) {
		NearestRingPos = &position;
	}

	// Interacted
	if (collider.checkCollision(_scene->camera.getCollider()) == true) {
		NearestRingPos = &Vector3(0, 0, 0);
		_scene->objBuilder.destroyObject(this);
		return true;
	}

	return false;
}
void Ring::collisionHit(Vector3& hitPos) {
}