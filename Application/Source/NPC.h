#ifndef NPC_H
#define NPC_H

#include "Object.h"

class NPC : public Object {

public:
	NPC(Scene* scene, Vector3 pos) : Object(scene, pos) {};
	~NPC() {};

	int getCurrentHealth() { return currentHP; };
	float getCurrentVelocity() { return _currentVelocity; };

	// Set the Health without any bounds checking
	void setHealth(int value);

	// Damage the NPC with proper bounds checking
	void reduceHealth(int value);

	// No bounds checking
	void setVelocity(float value);

	// No bounds checking
	void reduceVelocity(float value);

protected:
	int currentHP;
	float _currentVelocity = 0;


};
#endif