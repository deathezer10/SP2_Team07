#include "ObjectInteractor.h"
#include "Scene.h"



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

	Object* obj;
	objIterator = _objects.begin();

	for (objIterator; objIterator != _objects.end();) {

		obj = objIterator->second;

		if (obj->isActive() && obj->update() == false && obj->isCollidable() == true) { // Process collision
			Vector3 hitpos;
			Collider collider = obj->getCollider();

			// Render Box Collider of all Objects onto screen, Mesh: Cube Size: 0.5f, 0.5f, 0.5f
			if (UIManager::showDebugInfo == true) {

				obj->_scene->textManager.queueRenderMesh(UIManager::MeshQueue{

					obj->_scene->meshList[Scene::GEO_CUBE],
					collider.getPosition(),
					Vector3(0, 0, 0),
					Vector3(collider.bboxWidth, collider.bboxHeight, collider.bboxDepth)

				});

			}

			// Check for collision and only resolve it if the Object is not a Trigger
			if (collider.checkCollision(obj->_scene->camera.getCollider(), &hitpos) == true && collider.isTrigger() == false) {

				hitpos.Normalize();
				obj->_scene->camera.setVelocity(-1);
				obj->collisionHit(hitpos);
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