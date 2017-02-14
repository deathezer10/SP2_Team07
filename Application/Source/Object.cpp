#include "Object.h"
#include "Scene.h"



void Object::render() {
	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position.x, position.y, position.z);
	_scene->modelStack.Rotate(rotationX, 1, 0, 0);
	_scene->modelStack.Rotate(rotationY, 0, 1, 0);
	_scene->modelStack.Rotate(rotationZ, 0, 0, 1);
	_scene->modelStack.Scale(scale, scale, scale);
	_scene->RenderMesh(_scene->meshList[type], true);
	_scene->modelStack.PopMatrix();
}


bool Object::checkCollision(Vector3 &other, Vector3* hitDirection){

	float xMin = (-bboxWidth / 2) + position.x;
	float xMax = (bboxWidth / 2) + position.x;

	float yMin = (-bboxHeight / 2) + position.y;
	float yMax = (bboxHeight / 2) + position.y;

	float zMin = (-bboxDepth / 2) + position.z;
	float zMax = (-bboxDepth / 2) + position.z;

	if (other.x){
		// TODO: Collision
	}

	Vector3 hitDir = other - position;
	(*hitDirection) = hitDir;

	return false;
}