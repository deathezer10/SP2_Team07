#ifndef DOOR_H
#define DOOR_H


#include "Object.h"

// Space Bunker's Door, Interactable via keypress
class Door : public Object {
	
public:
	Door(Assignment03* scene, Vector3 pos);
	~Door() {};

	virtual void checkInteract();
	virtual void interact();

private:
	const float _interactDistance = 12.5f;
	const float _doorYLimit = 3;
	bool isDoorOpen = false;
	
};
#endif