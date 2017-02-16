#include "Application.h"
#include "Scene.h"
#include "Ring.h"

int Ring::RingCount = 0;

Ring::Ring(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_RING;
	rotationX = 90;
	RingCount=6;
	
};
void Ring::checkInteract()
{
	Vector3 distance = (position - _scene->camera.position);
	rotationY = -Math::RadianToDegree(atan2(distance.z, distance.x)) ;

	if ((position - _scene->camera.position).Length() < _interactDistance)
	{
		_scene->objBuilder.destroyObject(this);
		return;
	}
}
void Ring::interact()
{
}