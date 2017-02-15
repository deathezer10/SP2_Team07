#include "ObjectInteractor.h"
#include "Assignment03.h"


ObjectInteractor::ObjectInteractor() {
}

ObjectInteractor::~ObjectInteractor() {
	if (_objects.size()) {
		for (auto &i : _objects)
			delete i;
	}
	_objects.clear();
}

void ObjectInteractor::updateInteraction() {

	objIterator = _objects.begin();

	for (objIterator; objIterator != _objects.end();) {

		if ((*objIterator)->isCollidable()) {
			Vector3 gg;
			if ((*objIterator)->getCollider().checkCollision((*objIterator)->_scene->camera.getCollider(), &gg)) {
				gg.Normalize();
				(*objIterator)->_scene->camera.position += gg;
				(*objIterator)->_scene->camera.target += gg;
			}
		}

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