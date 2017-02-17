#include "Application.h"
#include "Scene.h"
#include "PowerUp.h"


PowerUp::PowerUp(Scene* scene, Vector3 pos,int powerType) : Object(scene, pos) {
	type = Scene::GEO_NONE;
	powertype = powerType;
	scale = 3;
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

void PowerUp::render() {


		_scene->modelStack.PushMatrix();
		_scene->modelStack.Translate(position.x, position.y, position.z);
		_scene->modelStack.Rotate(rotationY, 0, 1, 0);
		_scene->modelStack.Rotate(rotationX, 0, 0, 1);
		_scene->modelStack.Rotate(rotationZ, 1, 0, 0);
		_scene->modelStack.Scale(scale, scale, scale);

	switch (powertype)
	{
	case 0:
	{
		_scene->RenderMesh(_scene->meshList[Scene::GEO_SPEED], false);

		break;
	}
	case 1:
	{
		
		_scene->RenderMesh(_scene->meshList[Scene::GEO_BARRAGE], false);

		break;
	}
	case 2:
	{
		_scene->RenderMesh(_scene->meshList[Scene::GEO_REGEN], false);

		break;
	}
	}


		_scene->modelStack.PopMatrix();



	

}