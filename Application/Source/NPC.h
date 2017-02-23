#ifndef NPC_H
#define NPC_H

#include "Object.h"

class NPC : public Object {

public:
	NPC(Scene* scene, Vector3 pos);
	~NPC();

	virtual void render();

	int getCurrentHealth() { return currentHP; };
	float getCurrentVelocity() { return _currentVelocity; };

	// Set the Health without any bounds checking
	void setHealth(int value);

	// Damage the NPC with proper bounds checking, does not destroy the Object when health is <= 0
	void reduceHealth(int value);

	// No bounds checking
	void setVelocity(float value);

	// No bounds checking
	void reduceVelocity(float value);

	void toggleHealthbar(bool toggle){ _isHealthBarEnabled = toggle; };

protected:
	int currentHP;
	int defaultHP;

	float _currentVelocity = 0;
	bool _isHealthBarEnabled = true;


};
#endif