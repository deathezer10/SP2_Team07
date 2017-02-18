#include "Application.h"
#include "Scene.h"
#include "Rock.h"
#include "PlayerDataManager.h"

Rock::Rock(Scene* scene, Vector3 pos) : Object(scene, pos) {

	rotationY = Math::RandFloatMinMax(0, 360);
	const int sizeOffset = 3;
	scale = 5;
	collider.setBoundingBoxSize(Vector3(scale + sizeOffset, scale + sizeOffset, scale + sizeOffset));

	switch (Math::RandIntMinMax(0, 3)) {
	case 0:
		type = Scene::GEO_ROCK1;
		break;
	case 1:
		type = Scene::GEO_ROCK2;
		collider.setBoundingBoxSize(Vector3(scale + sizeOffset, scale + (scale * 3), scale + sizeOffset)); // special size for rock 2
		break;
	case 2:
		type = Scene::GEO_ROCK3;
		break;
	case 3:
		type = Scene::GEO_ROCK4;
		break;
	}
}


bool Rock::checkInteract() {

	rotationY += Math::RandFloatMinMax(1, 10)* _scene->_dt;

	// Rock collision
	if ((position - _scene->camera.target).Length() < _interactDistance) {

	}

	return false;
}

void Rock::collisionHit(Vector3& hitPos) {

	PlayerDataManager::getInstance()->damagePlayer(25);
	_scene->objBuilder.destroyObject(this);
	return;

}