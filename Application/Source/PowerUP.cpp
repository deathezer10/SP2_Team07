#include "Application.h"
#include "Scene.h"
#include "PowerUp.h"
#include "PlayerDataManager.h"

PowerUp::PowerUp(Scene* scene, Vector3 pos, int powerType) : Object(scene, pos) {
	type = Scene::GEO_NONE;
	powertype = powerType;
	scale = 3;
}


bool PowerUp::checkInteract() {
	if ((position - _scene->camera.position).Length() < _interactDistance) {
		switch (powertype) {
		case 0:
		{
			//Increases player speed to maximum for 5 seconds
			_scene->skillManager.activateSpeedBoost(5);
			_scene->objBuilder.destroyObject(this);
			return true;
			break;

		}
		case 1:
		{
			//Reduces the cooldown time between each bullet by 50% for 10 seconds
			_scene->skillManager.activateBarrage(((float)PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_speed / 2), 10);
			_scene->objBuilder.destroyObject(this);
			return true;
			break;
		}
		case 2:
		{
			//Restore the Fighter’s Shield &amp; base HP to full upon picking it up
			PlayerDataManager::getInstance()->getPlayerStats()->current_health = 100;
			PlayerDataManager::getInstance()->getPlayerStats()->current_shield = PlayerDataManager::getInstance()->getPlayerStats()->initial_shield_capacity;
			_scene->objBuilder.destroyObject(this);
			return true;
			break;
		}
		}
	}

	return false;
}
void PowerUp::collisionHit(Vector3& hitPos) {
}

void PowerUp::render() {


	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position.x, position.y, position.z);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationX, 0, 0, 1);
	_scene->modelStack.Rotate(rotationZ, 1, 0, 0);
	_scene->modelStack.Scale(scale, scale, scale);

	switch (powertype) {

	case 0:
		_scene->RenderMesh(_scene->meshList[Scene::GEO_SPEED], false);
		break;

	case 1:
		_scene->RenderMesh(_scene->meshList[Scene::GEO_BARRAGE], false);
		break;

	case 2:
		_scene->RenderMesh(_scene->meshList[Scene::GEO_REGEN], false);
		break;

	}
	_scene->modelStack.PopMatrix();

}