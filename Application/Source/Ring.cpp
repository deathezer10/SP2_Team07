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
	if ((position - _scene->camera.position).Length() < _interactDistance)
	{
		_scene->objBuilder.destroyObject(this);
		return;
	}
}
void Ring::interact()
{
}