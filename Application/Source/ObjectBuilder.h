#ifndef OBJECTBUILDER_H
#define OBJECTBUILDER_H


#include <vector>

#include "ObjectInteractor.h"


class Assignment03; // forward declaration


// Provide abstraction for creating interatable Objects in the Scene
class ObjectBuilder {

public:
	ObjectBuilder(Assignment03* scene);
	~ObjectBuilder() {};

	// Stores the Object into collection to allow interaction
    void createObject(Object* obj);

	// Remove the given Object from the vector
	void destroyObject(Object* obj);

	// Render all Objects in the collection into the Scene
	void renderObjects();

	ObjectInteractor objInteractor;

private:
	Assignment03* _scene;


};
#endif