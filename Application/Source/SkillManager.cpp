#include "Application.h"
#include "SkillManager.h"
#include "PlayerDataManager.h"
#include "Missile.h"

#include <map>

#include "GLFW\glfw3.h"


using std::multimap;


SkillManager::SkillManager(Scene* scene) {
	_scene = scene;
	pStat = PlayerDataManager::getInstance()->getPlayerStats();
};

void SkillManager::processSkills(double dt) {

	float _dt = (float)dt;
	_elapsedTime += dt;

	if (skillEnabled == false)
		return;

	processPowerUp();
	processPassiveSkill();

	// Start of Bullet logic
	// Using GLFW to get Mouse Down because Application::IsKeyPressed() is sometimes unreliable
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_1)) {

		Vector3 right = _scene->camera.playerView + (_scene->camera.getRight() * 0.8f);
		Vector3 left = _scene->camera.playerView - (_scene->camera.getRight() * 0.8f);
		float row = _scene->camera.getRoll();

		if (Application::IsKeyPressed('A')) {
			translateRight += _dt * 2;
			translateLeft -= _dt * 2;

			translateRight = Math::Clamp<float>(translateRight, 0, 0.6f);
			translateLeft = Math::Clamp<float>(translateLeft, -1.2f, 0.5f);
		}

		if (Application::IsKeyPressed('D')) {
			translateRight -= _dt * 2;
			translateLeft += _dt * 2;

			translateRight = Math::Clamp<float>(translateRight, -1.2f, 0.6f);
			translateLeft = Math::Clamp<float>(translateLeft, 0, 0.6f);
		}

		if (!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D')) {
			translateRight = 0;
			translateLeft = 0;
		}

		right += _scene->camera.getUp() * translateRight;
		left += _scene->camera.getUp() * translateLeft;

		if (_elapsedTime >= _nextBulletShootTime) {

			if (isShootingRight == true) {
				_scene->objBuilder.createObject(new Bullet(_scene, right, PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage));
				_nextBulletShootTime = _elapsedTime + PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_cooldown;
			}
			else {
				_scene->objBuilder.createObject(new Bullet(_scene, left, PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_damage));
				_nextBulletShootTime = _elapsedTime + PlayerDataManager::getInstance()->getPlayerStats()->current_bullet_cooldown;
			}
			isShootingRight = !isShootingRight;
		}
	}
	// End of Bullet logic

	// Start of Missile logic
	if (isTargetFullyLocked == true && lockedOnNPC != nullptr && !Application::IsKeyPressed(VK_SPACE)) { // Target is fully locked and <Spacebar> was released, fire missile!

		Vector3 missileSpawn = (_scene->camera.position + _scene->camera.getView().Normalized() * 2); // Spawn location for missile
		missileSpawn.y -= 1;
		_scene->objBuilder.createObject(new Missile(_scene, missileSpawn, lockedOnNPC), td_OBJ_TYPE::TYPE_SOLID);

		// Reset variables and add missile cooldown
		lockedOnNPC = nullptr;
		isTargetFullyLocked = false;
		rocketTargetCurrentSize = rocketTargetMaxSize; // Reset mesh to default size
		rocketNextShootTime = _scene->_elapsedTime + rocketCooldownTime; // Induce cooldown

	}
	else if (Application::IsKeyPressed(VK_SPACE) && rocketNextShootTime <= _scene->_elapsedTime) { // Find a valid NPC to target

		// Search for a valid target if no NPC is locked on
		if (lockedOnNPC == nullptr) {

			float nearestDistanceSquared = 999999; // cache nearest distance of the current npc

			// Retrieve all values that from key 'Enemy'
			auto mappy = _scene->objBuilder.objInteractor._objects.equal_range(td_OBJ_TYPE::TYPE_ENEMY);

			// Iterate through to find the nearest enemy NPC & if it's within field of vision
			for (multimap<td_OBJ_TYPE, Object*>::iterator it = mappy.first; it != mappy.second; ++it) {

				Object* obj = it->second;
				float distance = (obj->position - _scene->camera.playerView).LengthSquared(); // distance between this object and the player

				// No need for sqrt since we don't need absolute distance
				if (distance < nearestDistanceSquared) {

					// Select the NPC that is within FOV
					if (_scene->camera.playerView.IsFacingVector(obj->position, _scene->camera.getForward(), rocketTargetThreshold)) {
						lockedOnNPC = static_cast<NPC*>(obj);
						nearestDistanceSquared = distance;
						rocketTargetMaxSize = lockedOnNPC->getCollider().bboxHeight * 2;
						rocketTargetMinSize = lockedOnNPC->getCollider().bboxHeight / 2;
						rocketTargetCurrentSize = rocketTargetMaxSize;
					}

				}

			}
		}
		else if (lockedOnNPC->getCurrentHealth() > 0) { // Here we process locking on to the NPC

			if (isTargetFullyLocked == true) { // Target is fully locked on but <Spacebar> is still pressed
				// Render locked on Mesh on the NPC
				_scene->textManager.queueRenderMesh(UIManager::MeshQueue{
					_scene->meshList[Scene::GEO_CUBE],
					lockedOnNPC->position,
					Vector3(0),
					Vector3(rocketTargetMaxSize),
					false,
					false // disable wire frame
				});

			}
			// Constantly check if NPC is inside FOV
			else if (_scene->camera.playerView.IsFacingVector(lockedOnNPC->position, _scene->camera.getForward(), rocketTargetThreshold)) {

				// Render targeting Mesh on the NPC
				_scene->textManager.queueRenderMesh(UIManager::MeshQueue{
					_scene->meshList[Scene::GEO_CUBE],
					lockedOnNPC->position,
					Vector3(0),
					Vector3(rocketTargetCurrentSize)
				});

				rocketTargetCurrentSize -= ((rocketTargetMaxSize * 0.5f) * _scene->_dt); // Reduce targeting mesh size every frame

				if (rocketTargetCurrentSize <= rocketTargetMinSize) { // Check if target is fully locked on
					isTargetFullyLocked = true; // so that we stop rendering this mesh
					rocketTargetCurrentSize = rocketTargetMinSize; // Clamp it first
				}

			}
			else {
				// Reset locking system if NPC got out of sight
				lockedOnNPC = nullptr;
				isTargetFullyLocked = false;
				rocketTargetCurrentSize = rocketTargetMaxSize;

			}
		}
		else {
			// Reset locking system if enemy died
			lockedOnNPC = nullptr;
			isTargetFullyLocked = false;
			rocketTargetCurrentSize = rocketTargetMaxSize;
		}

	}
	else {
		// Reset locking if player released <Spacebar> before locking on
		lockedOnNPC = nullptr;
		isTargetFullyLocked = false;
		rocketTargetCurrentSize = rocketTargetMaxSize;
	}
	// End of Missile logic

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

		// Player got damaged again, reset the counter
		if (pStat->current_shield < shieldLastDamagedAmount) {
			shieldLastDamagedAmount = pStat->current_shield;
			_nextShieldRecoverTime = _elapsedTime + pStat->current_shield_recoveryRate; // Set cooldown time
		}
		else {
			pStat->current_shield += shieldRecoveryAmount * _scene->_dt; // Regenerate this amount every second
			shieldLastDamagedAmount += shieldRecoveryAmount * _scene->_dt;
		}

		// Shield is full now, clamp to max value and break out
		if (pStat->current_shield >= pStat->initial_shield_capacity) {
			pStat->current_shield = pStat->initial_shield_capacity;
			isShieldRecovering = false;
		}
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

float SkillManager::getCooldownTime() {
	return  Math::Clamp<float>(rocketNextShootTime - _scene->_elapsedTime, 0.0f, 20.f);
}