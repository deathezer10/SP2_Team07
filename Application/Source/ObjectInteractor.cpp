#include "ObjectInteractor.h"
#include "Assignment03.h"


ObjectInteractor::ObjectInteractor() {
}

ObjectInteractor::~ObjectInteractor() {
	if (_objects.size()){
		for (auto &i : _objects)
			delete i;
	}
	_objects.clear();
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