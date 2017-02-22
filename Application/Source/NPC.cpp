#include "NPC.h"
#include "Scene.h"


NPC::NPC(Scene* scene, Vector3 pos) : Object(scene, pos){

	scene->textManager.radar.addUnit(this);

}

NPC::~NPC(){
	_scene->textManager.radar.removeUnit(this);
};


void NPC::setHealth(int value) {
	currentHP = value;
}

void NPC::reduceHealth(int value) {

	if (value <= 0)
		return;

	currentHP -= value;
	currentHP = Math::Clamp(currentHP, 0, 500);

}


void NPC::setVelocity(float value) {
	_currentVelocity = value;
}


void NPC::reduceVelocity(float value) {
	_currentVelocity -= value;
}