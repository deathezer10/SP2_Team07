#ifndef OBJECT_H
#define OBJECT_H


#include "Application.h"
#include "Vector3.h"


class Assignment03;


// Abstract Base Class for the Scene's Interactable Objects
class Object {
	
public:
    Object(Assignment03* scene, Vector3 pos) : _scene(scene), position(pos) {};
    virtual ~Object() {};

	virtual void checkInteract() = 0;
    virtual void interact() = 0;
    virtual void render();

    Vector3 position;
    float rotationX = 0;
    float rotationY = 0;
    float rotationZ = 0;
    float scale = 1;
    unsigned type;

protected:
    Assignment03* _scene;
    bool isInteracted = false;

};
#endif