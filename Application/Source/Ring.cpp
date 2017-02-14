#include "Application.h"
#include "Scene.h"
#include "Ring.h"

Ring::Ring(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_CHAIR;
};

void Ring::checkInteract()
{

}
void Ring::interact()
{

}