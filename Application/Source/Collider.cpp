#include "Collider.h"
#include "Object.h"
#include "Scene.h"
#include "MeshBuilder.h"


Collider::Collider(Vector3* pos, float width, float height, float depth) {
	position = pos;
	bboxWidth = width;
	bboxHeight = height;
	bboxDepth = depth;
}

Collider::Collider(Object* obj, float width, float height, float depth) {
	_obj = obj;
	position = &obj->position;
	bboxWidth = width;
	bboxHeight = height;
	bboxDepth = depth;
}

Vector3 Collider::getBoxMin() {
	return Vector3((-bboxWidth / 2) + position->x,
				   (-bboxHeight / 2) + position->y,
				   (-bboxDepth / 2) + position->z);
}

Vector3 Collider::getBoxMax() {
	return Vector3((bboxWidth / 2) + position->x,
				   (bboxHeight / 2) + position->y,
				   (bboxDepth / 2) + position->z);
}

bool Collider::checkCollision(Collider &other, Vector3* hitDirection) {

	Vector3 myMin = getBoxMin();
	Vector3 myMax = getBoxMax();

	Vector3 otherMin = other.getBoxMin();
	Vector3 otherMax = other.getBoxMax();

	// Check if the box intercepts
	// I don't think nesting if-statements helps since C++ does short-circuit evaluations, but why not give it a shot :D
	if (myMax.x > otherMin.x && myMin.x < otherMax.x &&
		myMax.y > otherMin.y && myMin.y < otherMax.y &&
		myMax.z > otherMin.z && myMin.z < otherMax.z) {

		Vector3 hitDir = (*other.position) - (*position);

		if (hitDirection != nullptr)
			(*hitDirection) = hitDir.Normalized();

		return true;

	}

	return false;
}