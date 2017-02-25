#include "Application.h"
#include "SkillManager.h"
#include "PlayerDataManager.h"

#include "GLFW\glfw3.h"


SkillManager::SkillManager(Scene* scene) {
	_scene = scene;
	pStat = PlayerDataManager::getInstance()->getPlayerStats();
};

void SkillManager::processSkills(double dt) {

	_elapsedTime += dt;

	if (skillEnabled == false)
		return;

	processPowerUp();
	processPassiveSkill();

	// Using GLFW to get Mouse Down because Application::IsKeyPressed() is sometimes unreliable
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_1)) {
		if (_elapsedTime >= _nextShootTime) {
			_scene->objBuilder.createObject(new Bullet(_scene, _scene->camera.position, PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage));
			_nextShootTime = _elapsedTime + PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_cooldown;
		}
	}

	// Start of Godmode Activation
	if (!Application::IsKeyPressed('V')) {
		canGodmodePress = true;
	}

	if (Application::IsKeyPressed('V') && canGodmodePress) {
		isGodmodeActive = !isGodmodeActive;
		canGodmodePress = false;
	}

	if (isGodmodeActive) {
		pStat->current_shield = pStat->initial_shield_capacity;
		pStat->current_health = 100;
		_scene->textManager.queueRenderText(UIManager::Text{ "Godmode Activated", Color(.6f, .99f, .66f), UIManager::ANCHOR_CENTER_CENTER });
	}
	// End of Godmode Activation

}

void SkillManager::processPassiveSkill() {

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
		_scene->textManager.queueRenderText(UIManager::Text{ "Barrage Active", Color(0.97f, 1, 0.46f), UIManager::ANCHOR_TOP_LEFT });
	}

	// Reset speed
	if (_elapsedTime >= _nextSpeedBoostResetTime && isSpeedBoostActive) {
		isSpeedBoostActive = false;
		_scene->camera.setVelocity(40);
	}
	else if (isSpeedBoostActive) {
		_scene->textManager.queueRenderText(UIManager::Text{ "Speed Boost Active", Color(0.97f, 1, 0.46f), UIManager::ANCHOR_TOP_LEFT });
		_scene->camera.setVelocity(_scene->camera.getMaxVelocity()); // constantly set speed to maximum
		_scene->camera.setViewOffsetToMax();
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