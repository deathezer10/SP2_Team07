#ifndef OBJECTINTERACTOR_H
#define OBJECTINTERACTOR_H

#include <iterator>
#include <map>
#include "Object.h"
#include "Rock.h"
#include "Door.h"
#include "Slime.h"
#include "Bullet.h"
#include "Ring.h"
#include "PowerUp.h"

class Scene;


// Stores Objects in the Scene and update the interactions accordingly
class ObjectInteractor {

public:
	enum OBJ_TYPE {
		TYPE_SOLID = 0,
		TYPE_ENEMY
	};

	ObjectInteractor();
	~ObjectInteractor();

	std::multimap<OBJ_TYPE, Object*> _objects;

	// Process interactions for all Object
	void updateInteraction();

	// Updates the current iterator, used when a element is deleted from the vector
	void validateIterator(std::multimap<OBJ_TYPE, Object*>::iterator it);

private:
	std::multimap<OBJ_TYPE, Object*>::iterator objIterator;

	// Was the iterator updated?
	bool _iteratorUpdated = false;

};
#endif