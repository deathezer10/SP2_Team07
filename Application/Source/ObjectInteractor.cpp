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

		if (temp->checkInteract() == false && temp->isCollidable()) { // Process collision
			Vector3 hitpos;
			if (temp->getCollider().checkCollision(temp->_scene->camera.getCollider(), &hitpos)) {
				hitpos.Normalize();
				temp->_scene->camera.setVelocity(-1);
				temp->collisionHit(hitpos);
			}
		}

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