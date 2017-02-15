#ifndef OBJECTBUILDER_H
#define OBJECTBUILDER_H

#include "ObjectInteractor.h"



typedef ObjectInteractor::OBJ_TYPE td_OBJ_TYPE;

class Assignment03; // forward declaration

// Provide abstraction for creating interatable Objects in the Scene
class ObjectBuilder {

public:
	ObjectBuilder();
	~ObjectBuilder() {};

	// Stores the Object into collection to allow interaction
	void createObject(Object* obj, td_OBJ_TYPE objType = td_OBJ_TYPE::TYPE_SOLID);

	// Remove the given Object from the vector
	void destroyObject(Object* obj);

	// Render all Objects in the collection into the Scene
	void renderObjects();

	ObjectInteractor objInteractor;

private:
	Assignment03* _scene;


};
#endif