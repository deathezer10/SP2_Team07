#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"


// Azimuth Camera
class Camera2 : public Camera {
public:

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 defaultView;

	Camera2();
	~Camera2();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

};
#endif