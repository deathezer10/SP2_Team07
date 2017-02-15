#include "ObjectInteractor.h"
#include "Assignment03.h"


ObjectInteractor::ObjectInteractor() {
}

ObjectInteractor::~ObjectInteractor() {
	if (_objects.size()) {
		for (auto &i : _objects)
			delete i.second;
	}
	_objects.clear();
}

void ObjectInteractor::updateInteraction() {

	objIterator = _objects.begin();

	for (objIterator; objIterator != _objects.end();) {

		Object* temp = objIterator->second;

		if (temp->isCollidable()) {
			Vector3 gg;
			if (temp->getCollider().checkCollision(temp->_scene->camera.getCollider(), &gg)) {
				gg.Normalize();
				temp->_scene->camera.position += gg;
				temp->_scene->camera.target += gg;
			}
		}

		temp->checkInteract();

		if (!_iteratorUpdated) {
			++objIterator;
		}
		else {
			// skip increment since vector.erase() already returned the value of the next valid iterator
			_iteratorUpdated = false;
		}

	}

}


void ObjectInteractor::validateIterator(std::map<td_OBJ_TYPE, Object*>::iterator it) {
	objIterator = it;
	_iteratorUpdated = true;
}