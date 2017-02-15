#include "NPC.h"
#include "Scene.h"


void NPC::setHealth(int value) {
	currentHP = value;
}

void NPC::reduceHealth(int value) {

	if (value <= 0)
		return;

	currentHP -= value;
	currentHP = Math::Clamp(currentHP, 0, 500);

	if (currentHP <= 0) {
		_scene->objBuilder.destroyObject(this);
		return;
	}
}


void NPC::setVelocity(float value) {
	_currentVelocity = value;
}


void NPC::reduceVelocity(float value) {
	_currentVelocity -= value;
}