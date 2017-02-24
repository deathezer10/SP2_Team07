#include "ObjectBuilder.h"
#include <algorithm>



ObjectBuilder::ObjectBuilder() {
}

void ObjectBuilder::createObject(Object* obj, td_OBJ_TYPE objType) {
	objInteractor._objects.insert(std::pair<td_OBJ_TYPE, Object*>(objType, obj));
}

void ObjectBuilder::destroyObject(Object* obj) {

	std::multimap<td_OBJ_TYPE, Object*> &mappy = objInteractor._objects; // shortcut variable

	// Loop through the entire collection to erase the object
	for (auto &it = mappy.begin(); it != mappy.end();) {
		if (it->second == obj) {
			it = objInteractor._objects.erase(it);
			objInteractor.validateIterator(it);
			delete obj; // de-allocate memory
			return;
		}
		else {
			++it;
		}
	}


}

void ObjectBuilder::renderObjects() {
	// render all objects in map
	for (auto &i : objInteractor._objects) {
		i.second->render();
	}
}
