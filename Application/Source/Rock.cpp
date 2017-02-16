#include "Application.h"
#include "Scene.h"
#include "Rock.h"



static bool canPress = false;



Rock::Rock(Scene* scene, Vector3 pos) : Object(scene, pos) {
	collider.setBoundingBoxSize(Vector3(12, 12, 12));
	type = Scene::GEO_ROCK1;
	scale = 5;
	rotationY = Math::RandFloatMinMax(0, 180);
};


void Rock::checkInteract() {

	rotationY += Math::RandFloatMinMax(1, 10)* _scene->_dt;

	// Rock collision
	if ((position - _scene->camera.target).Length() < _interactDistance) {
		
	}
}

void Rock::interact() {


}