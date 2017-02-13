#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera2.h"
#include "Mtx44.h"

// FPS Camera
class Camera3 : public Camera2 {

	friend class CharacterController;

public:

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	const float skyboxSize = 1001.0f; // Distance of each skybox textures from the origin
	const float skyboxBound = 50.f; // Clamps position of the Camera to this value

	Vector3 getTarget() { return target; };
	Vector3 getUp() { return up; };
	Vector3 getPosition() { return position; };
	Vector3 getRight() { return right; };
	float getYaw() { return yaw; };
	float getPitch() { return pitch; };
	
	double getMouseMovedX() { return mouseMovedX; };
	double getMouseMovedY() { return mouseMovedY; };

	void disableMouse() { isMouseEnabled = false; };
	void enableMouse() { isMouseEnabled = true; };

	void ResetCursorVariables();

private:
	Vector3 view;
	Vector3 right;
	Vector3 defaultRight;

	// Current angle of camera rotation
	float yaw = 0;
	float pitch = 0;

	// Mouse direction moved, possible values: -1, 0, 1
	double mouseMovedX;
	double mouseMovedY;

	// Distance the Cursor moved from the current and last frame. Used to determine mouse sensitivity
	float mouseMovedDistanceX;
	float mouseMovedDistanceY; 

	// Updates the direction in which the cursor has moved
	void updateCursor(); 

	bool isMouseEnabled = true;

	// Maximum & Minimum height that the User can look
	float _MinYawAngle = -9999990.0f; // -50
	float _MaxYawAngle = 9999990.0f; // 50

	// Cursor's previous position
	double lastX = 0;
	double lastY = 0;


};
#endif