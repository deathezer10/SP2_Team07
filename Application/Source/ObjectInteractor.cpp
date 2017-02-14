#include <iterator>
#include "ObjectInteractor.h"
#include "Assignment03.h"


ObjectInteractor::ObjectInteractor() {
}

ObjectInteractor::~ObjectInteractor() {
	for (auto &i : _objects)
		delete i;
}

void ObjectInteractor::updateInteraction() {

	objIterator = _objects.begin();

	for (objIterator; objIterator != _objects.end();) {
		(*objIterator)->checkInteract();

		if (!_iteratorUpdated) {
			++objIterator;
		}
		else {
			// skip increment since vector.erase() already returned the value of the next valid iterator
			_iteratorUpdated = false; 
		}

	}

}


void ObjectInteractor::validateIterator(std::vector<Object*>::iterator it) {
	objIterator = it;
	_iteratorUpdated = true;
}