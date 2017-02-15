#include "Application.h"
#include "Scene.h"
#include "PowerUp.h"


PowerUp::PowerUp(Scene* scene, Vector3 pos,int powerType) : Object(scene, pos) {
	type = Scene::GEO_CUBE;
	powertype = powerType;
};
void PowerUp::checkInteract()
{
	if ((position - _scene->camera.position).Length() < _interactDistance)
	{
		switch (powertype)
		{
		case 0:
		{
			//Increases player speed by 50% for 10 seconds
			//currentvelocity += 50;
			_scene->objBuilder.destroyObject(this);
			return;
			break;

		}
		case 1:
		{
			//Reduces the cooldown time between each bullet by 50% for 5 seconds
			//currentvelocity += 50;
			_scene->objBuilder.destroyObject(this);
			return;
			break;
		}
		case 2:
		{
			//Restore the Fighter’s Shield &amp; base HP to full upon picking it up
			//currentvelocity += 50;
			_scene->objBuilder.destroyObject(this);
			return;
			break;
		}
		}
	}

}
void PowerUp::interact()
{
}