#ifndef NPC_H
#define NPC_H

#include "Object.h"

class NPC : public Object {

public:
	NPC(Scene* scene, Vector3 pos) : Object(scene, pos) {};
	~NPC() {};

	int getCurrentHealth() { return currentHP; };
	float getCurrentVelocity() { return _currentVelocity; };

	void setHealth(int value);
	void reduceHealth(int value);

	void setVelocity(float value);
	void reduceVelocity(float value);

protected:
	int currentHP = 50;
	float _currentVelocity = 0;


};
#endif