#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "Application.h"
#include "MatrixStack.h"
#include "Mesh.h"
#include "Geometry.h"
#include "CharacterRenderer.h"
#include "CharacterAnimator.h"

class Assignment03;

// Provides access to Character related Classes
class CharacterManager {


public:
	CharacterManager(Scene* scene, Mesh * meshlist, unsigned * mParameter, MS* mStack, MS* vStack, MS* pStack);
	~CharacterManager();

	CharacterRenderer& getRenderer() {
		return charRenderer;
	}

	CharacterAnimator& getAnimator() {
		return charAnimator;
	}

	bool isRockPickedUp = false;

	float getHealth() {
		return currentHealth;
	}

	void reduceHealth(float damage) {
		setHealth(currentHealth - damage);
	}

	void setHealth(float newHealth);

	float getCurrentScore() {
		return currentScore;
	}

	void setScore(float score);

	void increaseScore(float score);

	bool isGodmodeEnabled() {
		return isGodMode;
	}

	void setGodMode(bool trigger) {		
		isGodMode = trigger;
		setHealth(9999);
	}

private:
	CharacterRenderer charRenderer;
	CharacterAnimator charAnimator;

	float currentHealth = 100;
	float currentScore = 0;

	Scene* _scene;

	bool isGodMode = false;

};
#endif