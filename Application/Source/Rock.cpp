#include "Application.h"
#include "Scene.h"
#include "Rock.h"
#include "PlayerDataManager.h"

unsigned Rock::RockCount = 0;

Rock::Rock(Scene* scene, Vector3 pos) : Object(scene, pos) {

	++RockCount;
	rotationY = Math::RandFloatMinMax(0, 360);
	const int sizeOffset = 4;
	scale = 5;

	setCollision(true);
	collider.setBoundingBoxSize(Vector3(scale + sizeOffset, scale + sizeOffset, scale + sizeOffset));

	switch (Math::RandIntMinMax(0, 3)) {
	case 0:
		type = Scene::GEO_ROCK1;
		break;
	case 1:
		type = Scene::GEO_ROCK2;
		collider.setBoundingBoxSize(Vector3(scale + sizeOffset, scale + (scale * 5), scale + sizeOffset)); // special size for rock 2
		break;
	case 2:
		type = Scene::GEO_ROCK3;
		break;
	case 3:
		type = Scene::GEO_ROCK4;
		break;
	}
}


bool Rock::update() {

	rotationY += Math::RandFloatMinMax(1, 10)* _scene->_dt;
	
	return false;
}

void Rock::collisionHit(Vector3& hitPos) {

	PlayerDataManager::getInstance()->damagePlayer(_scene,_PlayerCollisionDamage);
	_scene->objBuilder.destroyObject(this);
	return;

}