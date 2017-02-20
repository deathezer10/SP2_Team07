#include "Application.h"
#include "SkillManager.h"
#include "PlayerDataManager.h"


void SkillManager::processSkills(double dt) {

	_elapsedTime += dt;

	if (skillEnabled == false)
		return;

	processPowerUp();
	processPassiveSkill();

	if (Application::IsKeyPressed(MK_LBUTTON)) {

		if (_elapsedTime >= _nextShootTime) {
			_scene->objBuilder.createObject(new Bullet(_scene, _scene->camera.playerView, PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage, false));
			_nextShootTime = _elapsedTime + PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_cooldown;
		}
	}

}

void SkillManager::processPassiveSkill() {

	PlayerStat* pStat = PlayerDataManager::getInstance()->getPlayerStats();

	// Shield Regeneration Logic
	if (pStat->current_shield < pStat->initial_shield_capacity && isShieldRecovering == false) {
		_nextShieldRecoverTime = _elapsedTime + pStat->current_shield_recoveryRate; // Set cooldown time
		shieldLastDamagedAmount = pStat->current_shield;
		isShieldRecovering = true;
	}
	else if (isShieldRecovering == true && _elapsedTime >= _nextShieldRecoverTime) {
		pStat->current_shield += shieldRecoveryAmount * _scene->_dt; // Regenerate this amount every second
		shieldLastDamagedAmount += shieldRecoveryAmount * _scene->_dt;

		// Shield is full now, clamp to max value and break out
		if (pStat->current_shield >= pStat->initial_shield_capacity) {
			pStat->current_shield = pStat->initial_shield_capacity;
			isShieldRecovering = false;
		}
	}

	// Player got damaged again, reset the counter
	if (pStat->current_shield < shieldLastDamagedAmount) {
		shieldLastDamagedAmount = pStat->current_shield;
		_nextShieldRecoverTime = _elapsedTime + pStat->current_shield_recoveryRate; // Set cooldown time
	}
	// End of Shield Regeneration Logic

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