#ifndef OBJ_COLLIDER_H
#define OBJ_COLLIDER_H

#include "Vector3.h"

class Object;

class Collider {

public:
	Collider(Vector3* pos, float width, float height, float depth);
	Collider(Object* obj, float width, float height, float depth);
	~Collider() {};

	float bboxWidth = 0;
	float bboxHeight = 0;
	float bboxDepth = 0;

	Vector3& getPosition() { return *position; };

	// Does this Collider push the player back?
	bool isTrigger() { return _isTrigger; };

	// Toggles whether this Collider pushes the player's Camera back upon collision
	void setTrigger(bool toggle) { _isTrigger = toggle; };

	void setBoundingBoxSize(Vector3 size) { bboxWidth = size.x; bboxHeight = size.y; bboxDepth = size.z; };

	// Compares the bounding box of this Object with the Target and returns true if they are interecepting
	// Output: Unit vector from the direction of the Target to this Object
	bool checkCollision(Collider &other, Vector3* hitDirection = nullptr);

private:
	Vector3* position;
	Object* _obj = nullptr;

	Vector3 getBoxMin();
	Vector3 getBoxMax();

	bool _isTrigger = false;

};
#endif