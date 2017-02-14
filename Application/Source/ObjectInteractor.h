#ifndef OBJECTINTERACTOR_H
#define OBJECTINTERACTOR_H

#include <iterator>
#include <vector>
#include "Object.h"
#include "Rock.h"
#include "Door.h"
#include "Slime.h"
#include "Bullet.h"


class Assignment03;


// Stores Objects in the Scene and update the interactions accordingly
class ObjectInteractor {
	
public:
	ObjectInteractor();
	~ObjectInteractor();

	std::vector<Object*> _objects;

	// Process interactions for all Object
	void updateInteraction();

	// Updates the current iterator, used when a element is deleted from the vector
	void validateIterator(std::vector<Object*>::iterator it);
	
private:
	Assignment03* _scene;
	std::vector<Object*>::iterator objIterator;

	// Was the iterator updated?
	bool _iteratorUpdated = false;
	
};
#endif