#include "Application.h"
#include "Scene.h"
#include "Rock.h"



static bool canPress = false;



Rock::Rock(Scene* scene, Vector3 pos) : Object(scene, pos) {
	collider.setBoundingBoxSize(Vector3(12, 12, 12));
	scale = 5;
	rotationY = Math::RandFloatMinMax(0, 180);
	switch (Math::RandIntMinMax(0, 3))
	{
	case 0:
	{
		type = Scene::GEO_ROCK1;
		break;
	}
	case 1:
	{
		type = Scene::GEO_ROCK2;
		break;
	}
	case 2:
	{
		type = Scene::GEO_ROCK3;
		break;
	}
	case 3:
	{
		type = Scene::GEO_ROCK4;
		break;
	}
	}
};


void Rock::checkInteract() {

	rotationY += Math::RandFloatMinMax(1, 10)* _scene->_dt;

	// Rock collision
	if ((position - _scene->camera.target).Length() < _interactDistance) {
		
	}
}

void Rock::interact() {

	
}