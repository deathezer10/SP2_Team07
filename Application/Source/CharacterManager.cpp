#include "CharacterManager.h"
#include "MeshBuilder.h"
#include "Assignment03.h"
#include "SceneGameOver.h"
#include "SceneManager.h"



CharacterManager::CharacterManager(Scene* scene) :
charRenderer(scene), charAnimator(scene) {
}

CharacterManager::~CharacterManager() {
}


void CharacterManager::setHealth(float newHealth) {

	if (isGodMode) {
		currentHealth = 9999;
		return;
	}

	currentHealth = Math::Clamp<float>(newHealth, 0, 100);

	// Player died
	if (currentHealth <= 0) {
        SceneManager::getInstance()->changeScene(new SceneGameover(SceneGameover::MENU_GAMEOVER));
	}
}

void CharacterManager::increaseScore(float score) {
	setScore(score + currentScore);

	// Victory, change scene
	if (currentScore >= 10) {
        SceneManager::getInstance()->changeScene(new SceneGameover(SceneGameover::MENU_VICTORY));
	}
}

void CharacterManager::setScore(float score) {
	currentScore = Math::Clamp<float>(score, 0, 100);
}