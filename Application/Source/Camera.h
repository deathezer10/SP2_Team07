#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera {
public:

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up) = 0;
	virtual void Reset() = 0;
	virtual void Update(double dt) = 0;

	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3& getPosition() { return position; };
	Vector3& getTarget() { return target; };
	Vector3& getUp() { return up; };

private:

};

#endif