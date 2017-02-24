#include "Scene.h"
#include "CargoShip.h"
#include "SceneManager.h"
#include "SceneGameOver.h"
#include "PlayerDataManager.h"

Vector3* CargoShip::HyperPosition;

CargoShip::CargoShip(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(6000);
	type = Scene::GEO_CARGOSHIP;
	scale = 5.0f;
	rotationY = 90.f;
	_interactDistance = scale;

	collider.setBoundingBoxSize(Vector3(scale + 10, scale + 4, scale + 15));

	HyperPosition = &position;

	isLightingEnabled = true;
};

bool CargoShip::checkInteract() {

	Destination -= _currentVelocity * _scene->_dt;

	if (Destination <= 0) {
		PlayerDataManager::getInstance()->getPlayerStats()->currency_earned += 500;
		SceneManager::getInstance()->changeScene(new SceneGameover("You have cleared this level, level 3 Unlocked!", SceneGameover::MENU_VICTORY, Scene::SCENE_CARGOSHIP, PlayerDataManager::getInstance()->getPlayerStats()->currency_earned));
		return true;
	}

	Vector3 distance = (position);//
	Vector3 unitDistance = distance.Normalized();


	// movement part
	if (Destination <= 0.0f) {
		_currentVelocity = 0;

	}
	else {
		_currentVelocity = _maxvelocity;
	}


	float moveZ = unitDistance.z  * _currentVelocity * _scene->_dt;
	position.z += moveZ;

	//scaling the healthbar to health left in percentage
	if (hp > Math::EPSILON)//so that the healthbar do not scale to x axis to 0 and crash the program
	{
		hp = (float)currentHP / (float)defaultHP;
	}

	//destroyed part
	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		SceneManager::getInstance()->changeScene(new SceneGameover("You failed: The Cargo Ship was destroyed!", SceneGameover::MENU_GAMEOVER, Scene::SCENE_CARGOSHIP, PlayerDataManager::getInstance()->getPlayerStats()->currency_earned/2));
		return true;
	}
	return false;
}

void CargoShip::collisionHit(Vector3& hitPos) {

}