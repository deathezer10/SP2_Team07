#include "ObjectBuilder.h"
#include "Assignment03.h"
#include <algorithm>

ObjectBuilder::ObjectBuilder() {
}


void ObjectBuilder::createObject(Object* obj) {
	objInteractor._objects.push_back(obj);
}

void ObjectBuilder::destroyObject(Object* obj) {

	std::vector<Object*> &vec = objInteractor._objects;

	objInteractor.validateIterator(vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end())); // re-validate the iterator after erasing element

	delete obj; // de-allocate memory

}

void ObjectBuilder::renderObjects() {
	for (auto &i : objInteractor._objects) {
		i->render();
	}
}
