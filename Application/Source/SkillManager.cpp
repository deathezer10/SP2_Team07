#include "Application.h"
#include "SkillManager.h"
#include "PlayerDataManager.h"


void SkillManager::processSkills(double dt) {

	_elapsedTime += dt;

	if (skillEnabled == false)
		return;

	processPowerUp();

	if (Application::IsKeyPressed(MK_LBUTTON)) {

		if (_elapsedTime >= _nextShootTime) {
			_scene->objBuilder.createObject(new Bullet(_scene, _scene->camera.playerView));
			_nextShootTime = _elapsedTime + PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_cooldown;
		}
	}

}

void SkillManager::processPowerUp() {


	// Reset bullet cooldown to default
	if (_elapsedTime >= _nextBarrageResetTime && isBarrageActive) {
		PlayerStat* stat = PlayerDataManager::getInstance()->getPlayerStats();
		isBarrageActive = false;
		stat->current_bullet_cooldown = stat->initial_bullet_cooldown;
	}
	else if (isBarrageActive) {
		_scene->textManager.queueRenderText(UIManager::Text{ "Barrage Active", Color(0, 0, 1), UIManager::ANCHOR_TOP_LEFT });
	}

	// Reset speed
	if (_elapsedTime >= _nextSpeedBoostResetTime && isSpeedBoostActive) {
		isSpeedBoostActive = false;
		_scene->camera.setVelocity(40);
	}
	else if (isSpeedBoostActive) {
		_scene->textManager.queueRenderText(UIManager::Text{ "Speed Boost Active", Color(0, 0, 1), UIManager::ANCHOR_TOP_LEFT });
		_scene->camera.setVelocity(_scene->camera.getMaxVelocity()); // constantly set speed to maximum
	}

}

void SkillManager::activateBarrage(float speed, float duration) {
	PlayerStat* stat = PlayerDataManager::getInstance()->getPlayerStats();

	isBarrageActive = true;
	_nextBarrageResetTime = _elapsedTime + duration;

	stat->current_bullet_cooldown -= speed;

	if (stat->current_bullet_cooldown < 0.1f)
		stat->current_bullet_cooldown = 0.1f;
}

void SkillManager::activateSpeedBoost(float duration) {
	PlayerStat* stat = PlayerDataManager::getInstance()->getPlayerStats();

	isSpeedBoostActive = true;
	_nextSpeedBoostResetTime = _elapsedTime + duration;

	_scene->camera.setVelocity(_scene->camera.getMaxVelocity()); // set speed to maximum
}